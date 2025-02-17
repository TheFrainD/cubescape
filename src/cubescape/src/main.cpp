#include "core/logger.h"
#include "core/window.h"

int main(int argc, char **argv) {
    core::Logger::Init(EXECUTABLE_NAME ".log");
    
    core::WindowSettings settings;
    settings.dimensions = {800, 600};
    settings.title      = EXECUTABLE_NAME;
    settings.multisample = 4;

    core::Window window(settings);
    if (window.Init() != core::WindowError::kOk) {
        LOG_CRITICAL("Failed to initialize window");
        return 1;
    }

    while (!window.ShouldClose()) {
        window.PollEvents();
        window.SwapBuffers();
    }

    window.Deinit();

    return 0;
}
