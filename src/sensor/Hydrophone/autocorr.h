/*
 * autocorr.h
 * ***********************
 * Header file describing the main routines used in the cross correlation tools that we're using
 *
 * Luke Renaud
 * Spring 2013
 * Edits:
 * 	02-09-2013:	Initial creation, Luke
 */
#include <complex.h>
#include <fftw3.h>

//#include "deffs.h"

#define WISDOM_FILENAME		"generated_wisdom.wis"

struct xcorr_rsc {
	fftw_complex	 *fft1; // FFT corresponding to signal 1
		// This will also become the location of the product of the FFTs later on
	fftw_complex	 *fft2; // FFT corresponding to signal 2
	// We would normally need to store the memory locations of the input and output signals
	// here as well, but the fftPlan is an opaque structure that contains this information.
	fftw_plan		fftPlan1;
	fftw_plan		fftPlan2;
	fftw_plan		ifftPlan;
	
	double			*xcorr; // Output location of the Xcorr result, we save this for scalling later.
	
	unsigned int	length;
};

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
		double *sig1, double *sig2, double *xcorrOut);

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
void clean_xcorr(struct xcorr_rsc *xcorrRSC);

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
void reverse_signal_xcorr(double *inSignal, unsigned int const length);

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
void compute_xcorr(struct xcorr_rsc *xcorrRSC);
