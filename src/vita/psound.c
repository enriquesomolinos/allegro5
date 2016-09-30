/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      VITA digital sound driver using the Allegro mixer.
 *      TODO: Audio input support.
 *
 *      By enrique somolinos.
 *
 *      See readme.txt for copyright information.
 */

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintvita.h"
#include <psp2/audioout.h>
#include <psp2/kernel/threadmgr.h>

#ifndef ALLEGRO_VITA
#error something is wrong with the makefile
#endif


#define IS_SIGNED TRUE           /* The VITA DSP reads signed sampled data */
#define SAMPLES_PER_BUFFER 1024
#define  FREQ 48000


static int vita_audio_channel_thread();
static int digi_vita_detect(int);
static int digi_vita_init(int, int);
static void digi_vita_exit(int);
//static int digi_vita_buffer_size();
static int digi_vita_set_mixer_volume(int);


static int vita_audio_on = FALSE;
static SceUID audio_thread_UID;
static int hw_channel;           /* The active VITA hardware output channel */
static short sound_buffer[2][SAMPLES_PER_BUFFER][2];
int curr_buffer=0;


DIGI_DRIVER digi_vita =
{
   DIGI_VITA,
   empty_string,
   empty_string,
   "PSP digital sound driver",
   0,
   0,
   MIXER_MAX_SFX,
   MIXER_DEF_SFX,

   digi_vita_detect,
   digi_vita_init,
   digi_vita_exit,
   digi_vita_set_mixer_volume,
   NULL,

   NULL,
   NULL,
   NULL,     //digi_vita_buffer_size,
   _mixer_init_voice,
   _mixer_release_voice,
   _mixer_start_voice,
   _mixer_stop_voice,
   _mixer_loop_voice,

   _mixer_get_position,
   _mixer_set_position,

   _mixer_get_volume,
   _mixer_set_volume,
   _mixer_ramp_volume,
   _mixer_stop_volume_ramp,

   _mixer_get_frequency,
   _mixer_set_frequency,
   _mixer_sweep_frequency,
   _mixer_stop_frequency_sweep,

   _mixer_get_pan,
   _mixer_set_pan,
   _mixer_sweep_pan,
   _mixer_stop_pan_sweep,

   _mixer_set_echo,
   _mixer_set_tremolo,
   _mixer_set_vibrato,
   0, 0,
   0,
   0,
   0,
   0,
   0,
   0
};



/* vita_audio_channel_thread:
 *  This vita thread manages the audio playing.
 */
static int vita_audio_channel_thread()
{
   while (vita_audio_on) {
      void *bufptr = &sound_buffer[curr_buffer];
      /* Asks to the Allegro mixer to fill the buffer */
      _mix_some_samples((uintptr_t)bufptr, 0, IS_SIGNED);
      /* Send mixed buffer to sound card */
	  
	  sceAudioOutOutput(hw_channel, bufptr);
	  
      curr_buffer = !curr_buffer;
   }

   sceKernelExitThread(0);

   return 0;
}



/* digi_vita_detect:
 *  Returns TRUE if the audio hardware is present.
 */
static int digi_vita_detect(int input)
{
   if (input) {
      ustrzcpy(allegro_error, ALLEGRO_ERROR_SIZE,
         get_config_text("Input is not supported"));
      return FALSE;
   }

   return TRUE;
}



/* digi_vita_init:
 *  Initializes the vita digital sound driver.
 */
static int digi_vita_init(int input, int voices)
{
   char digi_vita_desc[512] = EMPTY_STRING;
   char tmp1[256];

   if (input) {
      ustrzcpy(allegro_error, ALLEGRO_ERROR_SIZE,
         get_config_text("Input is not supported"));
      return -1;
   }

   
   //check type opened
   hw_channel = sceAudioOutOpenPort(SCE_AUDIO_OUT_PORT_TYPE_MAIN, SAMPLES_PER_BUFFER,FREQ,SCE_AUDIO_OUT_MODE_STEREO);
   if (hw_channel < 0) {
      ustrzcpy(allegro_error, ALLEGRO_ERROR_SIZE,
         get_config_text("Failed reserving hardware sound channel"));
      return -1;
   }

   vita_audio_on = TRUE;

   audio_thread_UID = sceKernelCreateThread("vita_audio_thread",
      (void *)&vita_audio_channel_thread, 0x19, 0x10000, 0,0, NULL);

   if (audio_thread_UID < 0) {
      ustrzcpy(allegro_error, ALLEGRO_ERROR_SIZE,
         get_config_text("Cannot create audio thread"));
      digi_vita_exit(FALSE);
      return -1;
   }

   if (sceKernelStartThread(audio_thread_UID, 0, NULL) != 0) {
      ustrzcpy(allegro_error, ALLEGRO_ERROR_SIZE,
         get_config_text("Cannot start audio thread"));
      digi_vita_exit(FALSE);
      return -1;
   }

   /* Allegro sound state variables */
   _sound_bits = 16;
   _sound_stereo = TRUE;
   _sound_freq = 44100;

   digi_vita.voices = voices;
   if (_mixer_init(SAMPLES_PER_BUFFER * 2, _sound_freq, _sound_stereo,
         (_sound_bits == 16), &digi_vita.voices))
   {
      ustrzcpy(allegro_error, ALLEGRO_ERROR_SIZE,
         get_config_text("Error initializing mixer"));
      digi_vita_exit(FALSE);
      return -1;
   }

   uszprintf(digi_vita_desc, sizeof(digi_vita_desc),
      get_config_text("%d bits, %d bps, %s"), _sound_bits,
      _sound_freq, uconvert_ascii(_sound_stereo ? "stereo" : "mono", tmp1));

   digi_vita.desc = digi_vita_desc;

   return 0;
}


/* digi_vita_exit:
 *  Shuts down the sound driver.
 */
static void digi_vita_exit(int input)
{
   if (input)
      return;

   vita_audio_on = FALSE;
   sceKernelDeleteThread(audio_thread_UID);
   sceAudioOutReleasePort(hw_channel);

   _mixer_exit();
}



/*static int digi_psp_buffer_size()
{
   return SAMPLES_PER_BUFFER;
}
*/


static int digi_vita_set_mixer_volume(int volume)
{	 
	int vols[2]={volume, volume};
	return sceAudioOutSetVolume(hw_channel, SCE_AUDIO_VOLUME_FLAG_L_CH|SCE_AUDIO_VOLUME_FLAG_R_CH, vols);
   
}

