/*
 * autocorr.c
 * ***********************
 * Routines used in the custom cross correlation tools that we're using
 *
 * Luke Renaud
 * Spring 2013
 * Edits:
 * 	02-09-2013:	Initial creation, Luke
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <complex.h>
#include <fftw3.h>

// These are for linux to test for wisdom
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#include "autocorr.h"
//#include "deffs.h"

/*
 * init_xcorr
 *
 * Inputs:
 * 	initalRSC		Structure for internal use in autocorrelation computation
 * 	len				length of the full cross-correlation (this should also be the length of the
 *						full padded input signals.
 * 	sig1			pointer to the input signal 1
 * 	sig2			pointer to the input signal 2
 * 	xcorrOut		pointer to the output memory location for the eventual output
 * Outputs:
 * 	none
 *
 * Desc:
 * This routine does initial setup for the cross-correlation routine we'll perform. When using the
 * library we've selected for fast Fourier transforms (the basis of the fast cross-correlation) we
 * first must run a planning stage. This stage looks at the length of the to be computed signals,
 * and the specific locations they've been given in memory, as well as the specifics of the host
 * hardware and attempts to compute an ideal or near ideal plan for how to actually compute the 
 * fast Fourier transforms we need. This takes a few seconds typically, but only needs to be
 * performed once.
 *
 * To facilitate this task, this routine allocates the internal memory that will be needed to 
 * internally compute the FFT's in the overall computation, and sets up the plans being used by the
 * computer.
 */
void init_xcorr(struct xcorr_rsc *initalRSC, unsigned int const len,
		double *sig1, double *sig2, double *xcorrOut) {
	bool wisdomExists = false;
	struct stat fileinfo;
	
	// The reason we can get away without using a full length complex allocation, is because we've
	// limited ourselves to real valued signals. In the real valued case we observe that the right
	// half of the transform is conjugate symmetric to the first half. The routines we're using take
	// advantage of this to simply not compute the second half speeding up computation. A smart
	// developer may then simply perform the conjugate operation themselves to compute the other
	// half of the array.
	//
	// We don't even do that here though. As all we want is element by element products of the
	// array, we can note that the product of a conjugates is the conjugate of the original product,
	// and thus we don't have any actual need of the number. Furthermore, the inversion routine we
	// use here assumes this conjugate symmetry, and just ignores longer data in the array. Thus,
	// in conversion of the first half of the array, everything is computed internally by the
	// fft routine here.
	if ((initalRSC->fft1 = fftw_alloc_complex(ceil(len/2+1))) == NULL) {
		printf("Error: Could not allocate output cross-correlation signal memory.\n");
		exit(1);
	}
	if ((initalRSC->fft2 = fftw_alloc_complex(ceil(len/2+1))) == NULL) {
		printf("Error: Could not allocate output cross-correlation signal memory.\n");
		fftw_free(initalRSC->fft1);
		exit(1);
	}

	// Save the memory locations should we decide to call upon them again later.
	initalRSC->length = len;
	initalRSC->xcorr = xcorrOut;
	
	// Seek an existing wisdom file before we try to manually regenerate another one.
	printf("Looking for existing wisdom (at "WISDOM_FILENAME")...\n");	
	if(0 == stat(WISDOM_FILENAME, &fileinfo)) {
		printf("Info: Wisdom found!\n");
		wisdomExists = true;
	} else {
		if (errno == ENOENT) {
			wisdomExists = false;
			printf("INFO: No wisdom, we must generate our own.\n");
		} else {
			printf("Misc error trying to open wisdom. FAIL!\n");
			exit(2);
		}
	}
	
	if(wisdomExists) {
		fftw_import_wisdom_from_filename(WISDOM_FILENAME);
	}
	
	printf("Gen Plan 1\n");
	// Generate the computational plan
	initalRSC->fftPlan1 = fftw_plan_dft_r2c_1d(len, sig1,
		initalRSC->fft1, FFTW_MEASURE);
	printf("Gen Plan 2\n");
	initalRSC->fftPlan2 = fftw_plan_dft_r2c_1d(len, sig2,
		initalRSC->fft2, FFTW_MEASURE);
		
	// And the inversion plan
	printf("Gen Plan 3\n");
	initalRSC->ifftPlan = fftw_plan_dft_c2r_1d(len, initalRSC->fft1,
		xcorrOut, FFTW_MEASURE);

	// If we generated our own wisdom, save it!
	if(!wisdomExists) {
		printf("Exporting new wisdom.\n");
		fftw_export_wisdom_to_filename(WISDOM_FILENAME);
	}

}

/*
 * clean_xcorr
 * 
 * Inputs
 * 	xcorrRSC		pointer to the resource structure being used
 * Outputs
 * 	none
 * 
 * Desc:
 * cleanup the memory being used internally by the cross-correlation routine.
 */ 
void clean_xcorr(struct xcorr_rsc *xcorrRSC) {
	fftw_destroy_plan(xcorrRSC->fftPlan1);
	fftw_destroy_plan(xcorrRSC->fftPlan2);
	fftw_destroy_plan(xcorrRSC->ifftPlan);
	fftw_free(xcorrRSC->fft1);
	fftw_free(xcorrRSC->fft2);
}

/*
 * reverse_signal_xcorr
 * 
 * Inputs
 * 	inSignal	pointer to the signal to be reversed
 *	length		length of the subsection of the signal to be reversed
 * Outputs
 * 	none
 * 
 * Desc:
 * In the implementation of the fast cross-correlation, one of the two input signals must be
 * reversed in time and padded prior to the computation of the Fourier transform. This routine
 * simply performs this time reversal.
 */ 
void reverse_signal_xcorr(double *inSignal, unsigned int const length) {
	double thirdHand;
	unsigned int i;
	// We'll keep a third hand, and swap the last and the first sample, moving in with each sample
	// until we are about to cross then stop.
	for(i = 0; i < length-1-i; i++) {
		thirdHand = inSignal[i];
		inSignal[i]	= inSignal[length-1-i];
		inSignal[length-1-i] = thirdHand;
	}
}

/*
 * compute_xcorr
 *
 * Inputs:
 *	xcorrRSC	pointer to computational object
 * Outputs:
 *	none
 *
 * Desc:
 * executes the computational steps needed to compute the cross correlation on the data present in
 * the memory locations outlined in the plans initially setup.
 */
void compute_xcorr(struct xcorr_rsc *xcorrRSC) {
	unsigned int i;	
	
	// Execute 1 and 2
	fftw_execute(xcorrRSC->fftPlan1);
	fftw_execute(xcorrRSC->fftPlan2);
	
	// Compute the product of the elements as needed by the fast Xcorr
	for (i = 0; i < xcorrRSC->length/2+1; i++) {
		xcorrRSC->fft1[i] = xcorrRSC->fft1[i] * xcorrRSC->fft2[i];
	}
	
	// And reverse back to time
	fftw_execute(xcorrRSC->ifftPlan);
	
	// Finally, we're going to scale the output values to produce the actual cross-correlation result.
	for (i = 0; i < xcorrRSC->length; i++) {
		xcorrRSC->xcorr[i] /= xcorrRSC->length;
	}
}
