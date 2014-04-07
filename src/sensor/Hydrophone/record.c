/*

This example reads from the default PCM device
and writes to standard output for # seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <sndfile.h>
#include <time.h>
#include <string.h>

int main() {
  long loops;
  int rc;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;
  
  int rc_2;
  int size_2;
  snd_pcm_t *handle_2;
  snd_pcm_hw_params_t *params_2;
  unsigned int val_2;
  int dir_2;
  snd_pcm_uframes_t frames_2;
  char *buffer_2;
  
  char *buffer_final;
  
  unsigned int i, frame_index;
  time_t current_time;
  
  char stime[80];
  char filename[80] = "\0";
  
  
	
////////////////////////////////////////////////////////////////////////
//SNDFILE* sf_open (const char *path, int mode, SF_INFO *sfinfo);
//sf_count_t  sf_write_int(SNDFILE *sndfile, int *ptr, sf_count_t items);
//int sf_close (SNDFILE *sndfile);

///////////////////////////////
/////////////////////////////////////////
	
  /* Open PCM device for recording (SND_PCM_STREAM_CAPUTRE). */
  rc = snd_pcm_open(&handle, "hw:0,0", SND_PCM_STREAM_CAPTURE, 0);//plughw:0,0
  rc_2 = snd_pcm_open(&handle_2, "hw:0,2", SND_PCM_STREAM_CAPTURE, 0);//Generic USB Audio Device:USB Audio(hw1:0)
  
  if (rc < 0) 
  {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
    exit(1);
  }
  
  if (rc_2 < 0) 
  {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
    exit(1);
  }
  
  

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);
  snd_pcm_hw_params_alloca(&params_2);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);
  snd_pcm_hw_params_any(handle_2, params_2);
  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
  snd_pcm_hw_params_set_access(handle_2, params_2, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
  snd_pcm_hw_params_set_format(handle_2, params_2, SND_PCM_FORMAT_S16_LE);
 
  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 2);
  snd_pcm_hw_params_set_channels(handle_2, params_2, 2);
	
  /* 96000 bits/second sampling rate (quality) */
  val = 96000;
  snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
  printf("%d\n", val);
  
  val_2 = 96000;
  snd_pcm_hw_params_set_rate_near(handle_2, params_2, &val_2, &dir_2);
  printf("%d\n", val_2);

  /* Set period size to 32 frames. */
  frames = 32;
  snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

  frames_2 = 32;
  snd_pcm_hw_params_set_period_size_near(handle_2, params_2, &frames_2, &dir_2);
	
  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) 
  {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
    exit(1);
  }
 
  rc_2 = snd_pcm_hw_params(handle_2, params_2);
  if (rc_2 < 0) 
  {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc_2));
    exit(1);
  }


  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  size = frames * 4; /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(size);

  snd_pcm_hw_params_get_period_size(params_2, &frames_2, &dir_2);
  size_2 = frames_2 * 4; /* 2 bytes/sample, 2 channels */
  buffer_2 = (char *) malloc(size_2);


	buffer_final = (char *) malloc(2 * size_2);

  /* Loop for 6 seconds */
  snd_pcm_hw_params_get_period_time(params, &val, &dir);
  snd_pcm_hw_params_get_period_time(params_2, &val_2, &dir_2);
    /* 2 (2000000)seconds in microseconds divided by period time */
  loops = 2e6 * 2 / val;

////////////////////////////////////////////////////////////////////////
	SF_INFO cfgOutFile = {
		.frames = loops * frames,
		.samplerate = 96000,
		.channels = 4,
		.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE
		};
		
////////////////////////////////////////////////////////////////////////
/* Obtain current time as seconds elapsed since the Epoch. */
current_time = time(NULL);
sprintf(stime, "%d", current_time);

//printf("time is: %s", stime);

//Create filename with timestamp
strcat(filename, "recording_");
strcat(filename, stime);

//printf("File name is: %s", filename);
	
SNDFILE* wave_file = sf_open(filename, SFM_WRITE, &cfgOutFile);

  while (loops > 0) 
  {
    loops--;
    rc = snd_pcm_readi(handle, buffer, frames);
	rc_2 = snd_pcm_readi(handle_2, buffer_2, frames_2);
      
    if (rc == -EPIPE) 
    {
      /* EPIPE means overrun */
      fprintf(stderr, "overrun occurred\n");
      snd_pcm_prepare(handle);
    } 
    else if (rc < 0) 
    {
      fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
    } 
    else if (rc != (int)frames) 
    {
      fprintf(stderr, "short read, read %d frames, expected %d \n", rc, (int)frames);
    }
   
    if (rc_2 == -EPIPE) 
    {
      /* EPIPE means overrun */
      fprintf(stderr, "overrun occurred\n");
      snd_pcm_prepare(handle_2);
    } 
    else if (rc_2 < 0) 
    {
      fprintf(stderr, "error from read: %s\n", snd_strerror(rc_2));
    } 
    else if (rc_2 != (int)frames_2) 
    {
      fprintf(stderr, "short read, read %d frames, expected %d \n", rc_2, (int)frames_2);
    }
   
   
////////////////////////////////////////////////////////////////////////   
//	buffer = 16 periods
//	1 period = 8 frames
//	1 frame = 2 samples of left+right
//	1 sample = 2 bytes of LSB and MSB, 1 byte each
////////////////////////////////////////////////////////////////////////
	if (rc != -EPIPE && rc_2 != -EPIPE) { // Only write if we don't have an overrun
	for(i = 0; i < rc; i++)								//go through all the samples
	{
		// Copy the frame from the first buffer, into the final buffer,
		// this is the first four bytes of the buffer. 2 samples
		for (frame_index = 0; frame_index < 4; frame_index++) 
		{
			buffer_final[8*i+frame_index] = buffer[4*i+frame_index];		//from buffer 1
			buffer_final[8*i+frame_index+4] = buffer_2[4*i+frame_index];	// from buffer 2
		}
	}

  rc = sf_writef_short(wave_file, (const short int *)buffer_final, frames);
	}
////////////////////////////////////////////////////////////////////////	


	if (rc != size/4)
      fprintf(stderr, "short write: wrote %d bytes, expected %d\n", rc, size/4);
  }

  sf_close(wave_file);

  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);

  return 0;
}
