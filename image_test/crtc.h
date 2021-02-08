/*
 * crtc.h
 *
 * Copyright (c) 2021 murabi10_c1815
 *
 */

void crtc_init();
void crtc();

void crtc_wait_vsync();
int vram_addressing(int x, int y);

void crtc_write(int x, int y, int stroke);
int crtc_read(int x, int y);

void crtc_write_meta(int x, int y, int data);
int crtc_read_meta(int x, int y);

#define VIDEO_OUT_PIN_BASE 6
#define SIZE 256
#define HALF_SIZE SIZE / 2
#define H_BLANK 54
#define V_BLANK 6
#define FRONT_POACH 8
#define H_SYNC 23