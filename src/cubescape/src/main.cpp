#include "core/event/key_event.h"
#include "core/input/input_manager.h"
#include "core/logger.h"
#include "core/window.h"

int main(int argc, char **argv) {
    core::Logger::Init(EXECUTABLE_NAME ".log");
    core::Context ctx;
    bool running {true};

    core::WindowSettings settings;
    settings.dimensions  = {800, 600};
    settings.title       = EXECUTABLE_NAME;
    settings.multisample = 4;

    core::Window window(ctx, settings);
    if (window.Init() != core::WindowError::kOk) {
        LOG_CRITICAL("Failed to initialize window");
        return 1;
    }

    core::InputManager::Init();

    struct Listener {
        void OnKeyPressed(const core::KeyPressedEvent &event) {
            LOG_INFO("Listener: Key pressed: {0}", static_cast<int>(event.key));
        };
    };

    Listener listener;
    ctx.event_bus.Connect<core::KeyPressedEvent, &Listener::OnKeyPressed>(
        listener);

    while (true) {
        window.PollEvents();

        if (window.ShouldClose()) {
            running = false;
        }

        if (core::InputManager::KeyPressed(core::Key::kEscape)) {
            running = false;
        }

        if (!running) {
            break;
        }

        window.SwapBuffers();
    }

    window.Deinit();

    return 0;
}
