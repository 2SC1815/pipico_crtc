/*
 * crtc.c
 *
 * Copyright (c) 2021 murabi10_c1815
 * 
 * Create an NTSC video signal with Raspberry pi pico to use your TV monitor.
 *
 */

#include "crtc.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "pico/multicore.h"
#include "crtc.pio.h"

/* This program creates a video signal by using DMA to transfer the contents of VRAM to PIO.
 * Since the 5th bit of VRAM contains a sync signal, VRAM has a prohibited area. */
uint8_t VRAM[(SIZE + H_BLANK)*(SIZE + V_BLANK)];

// Vertical sync signal
bool v_sync = true;

/* This function configures various settings for handling the monitor.
 * core1 is occupied after executing this function */
void crtc_init() {

    // The task of outputting the video signal is left to core1
    multicore_launch_core1(crtc);

    // Wait for VRAM to initialize to prevent the contents written to VRAM from being corrupted
    crtc_wait_vsync();
}

void crtc() {

    // Initialize VRAM with pedestal level
    for (int x = 0; x < SIZE + H_BLANK; ++x)
        for (int y = 0; y < SIZE + V_BLANK; ++y)
            VRAM[(y * (SIZE + H_BLANK)) + x] = 0b00010000;

    // Write Horizontal Sync signal
    for (int x = FRONT_POACH; x < (FRONT_POACH + H_SYNC); ++x)
        for (int y = 0; y < (SIZE + V_BLANK); ++y)
            VRAM[(y * (SIZE + H_BLANK)) + x] = 0b00000000;

    // Write Vertical Sync signal
    for (int x = 0; x < SIZE + H_BLANK; ++x)
        for (int y = 0; y < 4; ++y)
            VRAM[(y * (SIZE + H_BLANK)) + x] = 0b00000000;

    // PIO settings
    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio0, true);
    uint offset = pio_add_program(pio0, &crt_output_program);
    crt_output_program_init(pio0, sm, offset, VIDEO_OUT_PIN_BASE);
        
    // DMA settings
    int ch = dma_claim_unused_channel(true);
    dma_channel_config c = dma_channel_get_default_config(ch);
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_read_increment(&c, true);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, true));

    dma_channel_configure(
        ch,             // Channel to be configured
        &c,             // The configuration we just created
        &pio->txf[sm],  // Destinatinon is PIO
        &VRAM,          // Source is VRAM array
        count_of(VRAM), // Number of transfers
        false           // Transfer later
    );

    int count = count_of(VRAM);

    // Outputs video signal
    while (true) {
        v_sync = false;
        dma_channel_transfer_from_buffer_now(ch, &VRAM, count);
        while (dma_channel_is_busy(ch)){}
    }

}

// Wait for a vertical sync signal
void crtc_wait_vsync() {
    while(v_sync){}
    v_sync = true;
}

// Converts x, y coordinates to VRAM address
int vram_addressing(int x, int y) {
    return x + H_BLANK + (( y + V_BLANK ) * (SIZE + H_BLANK));
}

// Writes 1 pixel to VRAM at the specified coordinates.
void crtc_write(int x, int y, int stroke) {
    int addr = vram_addressing(x, y);
    VRAM[addr] = ((stroke & 0xF) | (VRAM[addr] & 0xF0));
}

// Gets 1 pixel from VRAM at the specified coordinates.
int crtc_read(int x, int y) {
    return VRAM[vram_addressing(x, y)] & 0xF;
}

// Writes 3 bit metadata to VRAM at the specified coordinates.
void crtc_write_meta(int x, int y, int data) {
    int addr = vram_addressing(x, y);
    VRAM[addr] = (((data & 0x7) << 5) | (VRAM[addr] & 0x1F));
}

// Gets 3 bit metadata from VRAM at the specified coordinates.
int crtc_read_meta(int x, int y) {
    return (VRAM[vram_addressing(x, y)] >> 5) & 0x7;
}

