#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <string.h>

#include <complex.h>
#include <fftw3.h>

#include "autocorr.h"

/*************************************************************************
 Typedefs
 ************************************************************************/
#define AMPLITUDE			0.8
#define PULSE_DURRATION		4e-3
#define	PULSE_FREQ			25e3
#define TIME_START1			0e-3
#define TIME_END1			(TIME_START1+PULSE_DURRATION)
#define TIME_START2			20e-3
#define TIME_END2			(TIME_START2+PULSE_DURRATION)

#define SAMPLING_RATE		96000
#define SIGNAL_LENGTH		SAMPLING_RATE
#define CROSSCOR_LENGTH		(2*SIGNAL_LENGTH-1)
#define CROSSCOR_SHORT		(CROSSCOR_LENGTH/2 + 1)

#define PI					3.141592653589793

/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct SoundData {
	int f;  //frames
	int sr;  //sample rate
	int c;  //channels
	int num_items;
	int **idata;  //Input data
} SoundData;

/*************************************************************************
Variables
************************************************************************/
SoundData Sdata;
/*************************************************************************
 Function Declarations
 ************************************************************************/
void parse_wav(SoundData *Sdata);

void parse_wav(SoundData *Sdata)
{
	SNDFILE *sf;
	SF_INFO info;
	int num_channels;
	int *buf;  //Buffer
	int num;
	int i,j;
	//int **idata;  //Input data
	//FILE *out;
	
	/* Open the WAV file. */
	info.format = 0;
	sf = sf_open("file.wav",SFM_READ,&info);
	if (sf == NULL)
	{
		printf("Failed to open the file.\n");
		exit(-1);
	}
	/* Print some of the info, and figure out how much data to read. */
	Sdata->f = info.frames;
	Sdata->sr = info.samplerate;
	Sdata->c = info.channels;
	//printf("frames=%d\n",Sdata->f);
	//printf("sample rate=%d\n",Sdata->sr);
	//printf("channels=%d\n",Sdata->c);
	Sdata->num_items = Sdata->f*Sdata->c;
	//printf("num_items=%d\n",Sdata->num_items);
	/* Allocate space for the data to be read, then read it. */
	buf = malloc(Sdata->num_items*sizeof(int));
	if((int) buf == ENOMEM)  //Check if we have enough memory
	{
		printf("Error: Not enough memory to allocate buffer memory");
		free(buf);
		exit(-1);
	}
	num = sf_read_int(sf,buf,Sdata->num_items);
	sf_close(sf);
	printf("Read %d items\n",num);
	
	/* Reorganize data for easier logical computations */
	Sdata->idata = (int **) malloc(Sdata->f*sizeof(int*));
	for (i=0; i< Sdata->f; i++)
	Sdata->idata[i] = malloc(Sdata->c*sizeof(int));
	if((int) Sdata->idata == ENOMEM)  //Check if we have enough memory
	{
		printf("Error: Not enough memory to allocate idata memory");
		free(buf);
		exit(-1);
	}

	for (i = 0; i < Sdata->f; i++)
	{
		for (j = 0; j < Sdata->c; j++)
		Sdata->idata[i][j] = buf[i*Sdata->c+j];
	}
	
	free(buf);  //Free buffer memory, we are done with it

	//test the function
	//printf("first row: %d %d %d %d \n",Sdata->idata[0][0],Sdata->idata[0][1],Sdata->idata[0][2],Sdata->idata[0][3]);
	
	/* Write the data to file data.out. */
	/*
	out = fopen("filedata.out","w");
	for (i = 0; i < num; i += c)
		{
		for (j = 0; j < c; ++j)
			fprintf(out,"%d ",buf[i+j]);
		fprintf(out,"\n");
		}
	fclose(out);
	*/

}





/*
 * zero_signal
 * 
 * Inputs:
 * 	length		number of elements following the pointer
 * 	signal		output signal
 * Outputs:
 * 	signal		output signal
 *
 * Desc:
 * This routine is simply used to null a freshly allocated block of memory. This simply takes each
 * element in the array, and manually loops through setting it to zero. If the length is WRONG, this
 * routine does nothing to make sure it does not overwrite other memory. Similarly, if the length is
 * short, then the array simply does not zero the values following it. This can be used to zero
 * short parts of a signal if the pointer location and length is cleverly used.
 */
void zero_signal(unsigned int const length, double *signal) {
	unsigned int i;
	for (i = 0; i < length; i++) {
		signal[i] = 0.0;	
	}
}

/*
 * setup_signal
 * 
 * Inputs:
 * 	n1, n2		length of the first and second signals (ideally should be the same)		
 *	sig1		pointer to the first signal (i.e. the perfect reference)
 *	sig2		pointer to the second signal (i.e. the recorded sound)
 * Outputs:
 *	sig1		pointer to the first signal (i.e. the perfect reference)
 *	sig2		pointer to the second signal (i.e. the recorded sound)
 *
 * Desc:
 * This routine generates the two signals we'd like to generate the cross-correlation between. To do
 * this we generate our cosine pulse at the beginning, over a fixed duration for the first signal, 
 * and repeat this process with a fixed delay for the second signal. This routine does nothing to
 * actually prep the signals to be passed to the cross-correlation routine, it simply generates the
 * desired signals
 *
 * This is done by converting each sample (given the sampling rate) to a time, and comparing the
 * time of the current sample to the desired start and stop times of the signal. If we're inside
 * the signal's time window, we simply insert the time shifted cosine value into the array, other-
 * wise the value is not computed. This should leave it defaulted to zero assuming the signal was
 * properly zeroed prior to being generated here.
 */ 
 void setup_signals(	unsigned int const n1, double *sig1,
 						unsigned int const n2, double *sig2) {
 	unsigned int i;
 	
 	// Generate signal 1
	for(i = 0; i < n1; i++) {
		// Ensure current sample is between the starting and stopping sample numbers
		if( i >= round(TIME_START1*SAMPLING_RATE) && i < round(TIME_END1*SAMPLING_RATE)) {
			// and save the pulse value for the current time sample
			sig1[i] = AMPLITUDE*cos(2*PI*PULSE_FREQ * (1.0*i/SAMPLING_RATE));
		}
	} 	

	for(i = 0; i < n1; i++) {
		if( i >= round(TIME_START2*SAMPLING_RATE) && i < round(TIME_END2*SAMPLING_RATE)) {
			sig2[i] = AMPLITUDE*cos(2*PI*PULSE_FREQ * ((1.0*i/SAMPLING_RATE) - TIME_START2));
		}
	} 	
}

void write_signal(char const filename[const], unsigned int const length, double *data) {
	unsigned int i;
	char filename_txt[FILENAME_MAX-4];
	char filename_bin[FILENAME_MAX-4];
	FILE *h;

	strcpy(filename_txt,filename);
	strcpy(filename_bin,filename);
	strcat(filename_txt,".txt");
	strcat(filename_bin,".dat");
	
	if((h = fopen(filename_txt, "w")) != NULL) {
		for (i = 0; i < length; i++)
			fprintf(h, "%+.10f\n", data[i]);	
		fclose(h);
	}

	if((h = fopen(filename_bin, "wb")) != NULL) {
		fwrite(data, sizeof(double), length, h);
		fclose(h);
	}
}

int main()
{
	// First we need our two time domain input signals
	double *sig1, *sig2;
	// also we need the output cross correlation signal
	double *xcorr;
	// And we'll have a special structure we need for our actual computation tool
	struct xcorr_rsc xcorr_data;
	
	// Allocate memory for all of our signals. Note that because our signal will be used for cross-
	// correlation computation, we'll need to make sure that the memory is long enough to keep
	// everything padded for the fast autocorrelation computation.
	printf("Allocating memory...\n");
	if ((sig1 = calloc(CROSSCOR_LENGTH, sizeof(double))) == NULL) {
		printf("Error: Could not allocate signal 1 memory.\n");
		exit(1);
	}
	if ((sig2 = calloc(CROSSCOR_LENGTH, sizeof(double))) == NULL) {
		printf("Error: Could not allocate signal 2 memory.\n");
		free(sig1);
		exit(1);
	}
	if ((xcorr = calloc(CROSSCOR_LENGTH, sizeof(double))) == NULL) {
		printf("Error: Could not allocate output cross-correlation signal memory.\n");
		free(sig1);
		free(sig2);
		exit(1);
	}
	parse_wav(&Sdata);
	
		// To prepare for the fast-correlation, we need to generate the computational plans first
	// This must be done BEFORE we actually pass in the signal as the computational plans run
	// slightly faster if we allow them to overwrite the memory in their tests. This can be avoided
	// with a flag, but to take advantage of the speed boost all we have to do is a setup the
	// signal after the fact rather than before, there is little reason to not take advantage of it.
	printf("Generating computational plans...\n"
		"\tThis takes a LONG TIME on slower systems. Consider:\n"
		"\t8-core 3.6GHz i7     ~10 seconds\n"
		"\t2-core 1.6GHz Atom   ~8 minutes\n");
	init_xcorr(&xcorr_data, CROSSCOR_LENGTH, sig1, sig2, xcorr);
	
	// Now we setup our input signals
	printf("Generating input signals...\n");
	zero_signal(CROSSCOR_LENGTH, sig1);
	zero_signal(CROSSCOR_LENGTH, sig2);
	zero_signal(CROSSCOR_LENGTH, xcorr);
	setup_signals(SIGNAL_LENGTH, sig1, SIGNAL_LENGTH, sig2);	
	
	// Reverse the second signal in preparation for the computation
	reverse_signal_xcorr(sig2, SIGNAL_LENGTH);
	
	write_signal("signal1",CROSSCOR_LENGTH, sig1);
	write_signal("signal2",CROSSCOR_LENGTH, sig2);
	
	// AND EXECUTE!
	printf("Computing...\n");
	compute_xcorr(&xcorr_data);
	
	write_signal("xcorr_result",CROSSCOR_LENGTH, xcorr);
		
	
	// Clean up everything
	printf("Cleaning up...\n");

	free(Sdata.idata);
	clean_xcorr(&xcorr_data);
	free(sig1);
	free(sig2);
	free(xcorr);

	printf("Bye bye.\n");
	return 0;
}
