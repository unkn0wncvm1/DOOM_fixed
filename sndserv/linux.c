// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id: linux.c,v 1.3 1997/01/26 07:45:01 b1 Exp $
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
//
// $Log: linux.c,v $
// Revision 1.3  1997/01/26 07:45:01  b1
// 2nd formatting run, fixed a few warnings as well.
//
// Revision 1.2  1997/01/21 19:00:01  b1
// First formatting run:
//  using Emacs cc-mode.el indentation for C++ now.
//
// Revision 1.1  1997/01/19 17:22:45  b1
// Initial check in DOOM sources as of Jan. 10th, 1997
//
//
// DESCRIPTION:
//	UNIX, soundserver for Linux i386.
//
//-----------------------------------------------------------------------------

static const char rcsid[] = "$Id: linux.c,v 1.3 1997/01/26 07:45:01 b1 Exp $";
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>
#include "soundsrv.h"

extern snd_pcm_t *handle;
extern snd_pcm_hw_params_t *params;
extern unsigned int sampleRate;
extern int dir;
extern snd_pcm_uframes_t frames;

void myioctl(int fd, int command, int *arg) {
    int rc = ioctl(fd, command, arg);
    if (rc < 0) {
        fprintf(stderr, "ioctl(dsp,%d,arg) failed\n", command);
        fprintf(stderr, "errno=%d\n", errno);
        exit(-1);
    }
}

void I_InitMusic(void) {
    // ALSA does not need specific initialization for music
}

void I_InitSound(int samplerate, int samplesize) {
    // Initialize ALSA
    if (snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        fprintf(stderr, "Unable to open PCM device\n");
        exit(1);
    }

    snd_pcm_hw_params_malloc(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 2);
    snd_pcm_hw_params_set_rate_near(handle, params, &sampleRate, &dir);

    if (snd_pcm_hw_params(handle, params) < 0) {
        fprintf(stderr, "Unable to set HW parameters\n");
        exit(1);
    }

    snd_pcm_hw_params_free(params);
    frames = 32;
}

void I_SubmitOutputBuffer(void *samples, int samplecount) {
    snd_pcm_writei(handle, samples, samplecount);
}

void I_ShutdownSound(void) {
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
}

void I_ShutdownMusic(void) {
    // ALSA does not need specific shutdown for music
}
