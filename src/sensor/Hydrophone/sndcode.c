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
//#define CROSSCOR_LENGTH		(2*SIGNAL_LENGTH-1)
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
	double **idata;  //Input data
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
	double *buf;  //Buffer
	int num;
	int i,j;
	//int **idata;  //Input data
	FILE *out;
	
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
	buf = (double*)malloc(Sdata->num_items*sizeof(double));
	if((int) buf == ENOMEM)  //Check if we have enough memory
	{
		printf("Error: Not enough memory to allocate buffer memory");
		free(buf);
		exit(-1);
	}
	num = sf_read_double(sf,buf,Sdata->num_items);
	sf_close(sf);
	printf("Read %d items\n",num);
	
	/* Reorganize data for easier logical computations */
	Sdata->idata = (double **) malloc(Sdata->c*sizeof(double*));
	for (i=0; i< Sdata->c; i++)
	Sdata->idata[i] = (double*)calloc((2*Sdata->f-1),sizeof(double));
	if((int) Sdata->idata == ENOMEM)  //Check if we have enough memory
	{
		printf("Error: Not enough memory to allocate idata memory");
		free(buf);
		exit(-1);
	}

	for (i = 0; i < Sdata->c; i++)
	{
		for (j = 0; j < Sdata->f; j++)
		Sdata->idata[i][j] = buf[i*Sdata->c+j];
	}

	//test the function
	//printf("first row: %lf %lf %lf %lf \n",Sdata->idata[0][0],Sdata->idata[0][1],Sdata->idata[0][2],Sdata->idata[0][3]);
	
	/* Write the data to file data.out. */
	/*
	out = fopen("filedata2.out","w");
	for (i = 0; i < num; i += Sdata->c)
		{
		for (j = 0; j < Sdata->c; ++j)
			fprintf(out,"%lf ",buf[i+j]);
		fprintf(out,"\n");
		}
	fclose(out);
	*/
	free(buf);  //Free buffer memory, we are done with it

}

void write_signal(char const filename[], unsigned int const length, double *data) {
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
	// We need the output cross correlation signal
	double *xcorr;
	// And we'll have a special structure we need for our actual computation tool
	struct xcorr_rsc xcorr_data;
	int CROSSCOR_LENGTH;
	int start_time,end_time;
	
	start_time = time(NULL);

	printf("Parsing WAV File...\n");
	parse_wav(&Sdata);

	CROSSCOR_LENGTH = 2*Sdata.f-1;
	printf("Allocating Memory...\n");
	if ((xcorr = (double*)calloc(CROSSCOR_LENGTH, sizeof(double))) == NULL) {
		printf("Error: Could not allocate output cross-correlation signal memory.\n");
		free(Sdata.idata);
		exit(1);
	}

	
	
	// To prepare for the fast-correlation, we need to generate the computational plans first
	// This must be done BEFORE we actually pass in the signal as the computational plans run
	// slightly faster if we allow them to overwrite the memory in their tests. This can be avoided
	// with a flag, but to take advantage of the speed boost all we have to do is a setup the
	// signal after the fact rather than before, there is little reason to not take advantage of it.
	printf("Generating computational plans...\n"
		"\tThis takes a LONG TIME on slower systems. Consider:\n"
		"\t8-core 3.6GHz i7     ~10 seconds\n"
		"\t2-core 1.6GHz Atom   ~8 minutes\n");
	init_xcorr(&xcorr_data, CROSSCOR_LENGTH, Sdata.idata[0], Sdata.idata[1], xcorr);
	
	// Reverse the second signal in preparation for the computation
	printf("Reversing signal...\n");
	reverse_signal_xcorr(Sdata.idata[1], Sdata.f);
	
	printf("writing signals...\n");
	//write_signal("signal1",CROSSCOR_LENGTH, Sdata.idata[0]);
	//write_signal("signal2",CROSSCOR_LENGTH, Sdata.idata[1]);
	
	// AND EXECUTE!
	printf("Computing...\n");
	compute_xcorr(&xcorr_data);
	
	printf("Writing final output...\n");
	//write_signal("xcorr_result",CROSSCOR_LENGTH, xcorr);
		
	
	// Clean up everything
	printf("Cleaning up...\n");

	
	clean_xcorr(&xcorr_data);
	//free(sig1);
	//free(sig2);
	free(xcorr);

	free(Sdata.idata);
	printf("Program Complete\n");
	
	end_time = time(NULL);
	printf("Program Run time: %d seconds\n", (end_time-start_time));
	return 0;
}
