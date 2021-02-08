add_executable(game_of_life game_of_life.c crtc.c)

pico_generate_pio_header(game_of_life ${CMAKE_CURRENT_LIST_DIR}/crtc.pio)

target_link_libraries(game_of_life 
        pico_stdlib 
        hardware_dma 
        hardware_pio
        pico_multicore
        )

pico_add_extra_outputs(game_of_life)

#example_auto_set_url(game_of_life)