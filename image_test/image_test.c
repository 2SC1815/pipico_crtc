/*
 * image_test.c
 *
 * Copyright (c) 2021 murabi10_c1815
 *
 * Displays a black-and-white image with a 
 * resolution of 256 * 256 and 16 gradations 
 * on an NTSC TV monitor.
 *
 * Electrical connections:
 * 
 * GND--------+
 *            |
 *            R1
 *            |
 * GP6---R1---+
 *            |
 *            R2
 *            |
 * GP7---R1---+
 *            |
 *            R2
 *            |
 * GP8---R1---+
 *            |
 *            R2
 *            |
 * GP9---R1---+
 *            |    3V3 or VBUS
 *            R3       |
 *            |        C
 * GND---R3---+------B[Q1]
 *            |        E
 *            |        |
 * GP10--R4---+        @ OUTPUT
 * 
 * R1: 560 Ω
 * R2: 270 Ω
 * R3: 470 Ω
 * R4: 820 Ω
 * Q1: 2SC1815 or equivalent
 * 
 * If the input terminal of the monitor is high impedance,
 * the image may be distorted.
 * In that case, insert a resistor of about
 * 100Ω between the Emitter of Q1 and GND.
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "crtc.h"

/* This include file contains images in binary format. 
 * How to change the image is described in this file  */
#include "image.h"

int main() {

    /* This function configures various settings for handling the monitor.
     * core1 is occupied after executing this function */
    crtc_init();
    
    /* Display the image. Since the displayed pixels are
     * 16 gradations = 4 bits, the image data contains 2 pixels per byte. */
    for(int x = 0; x < SIZE; x+=2) {
        for(int y = 0; y < SIZE; y++) {
            uint8_t pixels = Image[(x / 2) + (y * HALF_SIZE)];
            crtc_write(x  , y, pixels >> 4);
            crtc_write(x+1, y, pixels     );
        }
    }

}