/*
 * game_of_life.c
 *
 * Copyright (c) 2021 murabi10_c1815
 *
 * Conway's game of life
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "crtc.h"

int random_xorsft(){
    static uint32_t x = 123456789;
    static uint32_t y = 362436069;
    static uint32_t z = 521288629;
    static uint32_t w = 88675123;
    uint32_t t;
    t = x ^ (x<<11);
    x = y; y = z; z = w;
    w ^= t ^ (t>>8) ^ (w>>19);
    return w & 0xFF;
}

int main() {

    crtc_init();

    for(int x = 0; x < SIZE; x++)
        for(int y = 0; y < SIZE; y++)
            crtc_write(x, y, (random_xorsft()&1) ? 0xF : 0x0);

    while(true) {

        for(int i = 0; i < 100; i++) {

            for(int x = 0; x < SIZE; x++) {
                for(int y = 0; y < SIZE; y++) {
                    int n =
                    (crtc_read(x-1, y-1)&1) + (crtc_read(x, y-1)&1) + (crtc_read(x+1, y-1)&1) +
                    (crtc_read(x-1, y  )&1) +                                (crtc_read(x+1, y  )&1) +
                    (crtc_read(x-1, y+1)&1) + (crtc_read(x, y+1)&1) + (crtc_read(x+1, y+1)&1) ;

                    if (crtc_read(x, y) == 0xF) {
                        // live cell
                        crtc_write_meta(x, y, (n == 3 || n == 2) ? 1 : 0);
                    } else {
                        // dead cell
                        crtc_write_meta(x, y, (n == 3) ? 1 : 0);
                    }
                }
            }

            crtc_wait_vsync();

            for(int x = 0; x < SIZE; x++)
                for(int y = 0; y < SIZE; y++)
                    crtc_write(x, y, (crtc_read_meta(x, y) == 1) ? 0xF : 0x0);

        }

        /* If left unattended for a long time, it will become saturated,
         * so spawn gliders at random positions every 100 generations.   */

        int xg = random_xorsft(), yg = random_xorsft();

        crtc_write(xg + 2, yg + 1, 0xF);
        crtc_write(xg + 3, yg + 2, 0xF);
        crtc_write(xg + 1, yg + 3, 0xF);
        crtc_write(xg + 2, yg + 3, 0xF);
        crtc_write(xg + 3, yg + 3, 0xF);

    }

}