/*

This example reads standard from input and writes
to the default PCM device for # seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API


#include <alsa/asoundlib.h>
#include <stdio.h>
#include <sndfile.h>

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

  /* Open PCM device for playback (SND_PCM_STREAM_PLAYBACK). */
  rc = snd_pcm_open(&handle, "pulse", SND_PCM_STREAM_PLAYBACK, 0);
 
  if (rc < 0) 
	{
	  fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
	  exit(1);
	}

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

  /* Two channels (stereo) */
  snd_pcm_hw_params_set_channels(handle, params, 4);

  /* 96000 bits/second sampling rate (quality) */
  val = 192000;
  snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

  /* Set period size to 32 frames. */
  frames = 32;
  snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) 
	{
	fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
	exit(1);
	}

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  size = frames * 8; /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(size);

  /* Loop for 6 seconds */
  snd_pcm_hw_params_get_period_time(params, &val, &dir);
  /* 6 seconds (6000000) in microseconds divided by period time */
  loops = 6000000 * 2 / val;

  while (loops > 0) 
  {
    loops--;
    rc = read(0, buffer, size);
    if (rc == 0) 
    {
      fprintf(stderr, "end of file on input\n");
      break;
    } else if (rc != size) {
      fprintf(stderr,
              "short read: read %d bytes\n", rc);
    }
    rc = snd_pcm_writei(handle, buffer, frames);
    if (rc == -EPIPE) 
    {
      /* EPIPE means underrun */
      fprintf(stderr, "underrun occurred\n");
      snd_pcm_prepare(handle);
    } 
    else if (rc < 0) 
    {
      fprintf(stderr, "error from writei: %s\n", snd_strerror(rc));
    }  
    else if (rc != (int)frames) 
    {
      fprintf(stderr, "short write, write %d frames\n", rc);
    }
  }

  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);

  return 0;
}