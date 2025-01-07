#include <stdio.h>

#include "core/log.h"
#include "graphics/window.h"

#define EXECUTABLE_NAME "Cubescape"

int main(int argc, char **argv) {
    log_info("%s starting up...", EXECUTABLE_NAME);

    create_window(800, 600, EXECUTABLE_NAME);

    while (!window_should_close()) {
        poll_events();
        swap_buffers();
    }
    
    destroy_window();
    return 0;
}