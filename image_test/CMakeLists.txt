add_executable(image_test image_test.c crtc.c)

pico_generate_pio_header(image_test ${CMAKE_CURRENT_LIST_DIR}/crtc.pio)

target_link_libraries(image_test 
        pico_stdlib 
        hardware_dma 
        hardware_pio
        pico_multicore
        )


pico_add_extra_outputs(image_test)

#example_auto_set_url(image_test)