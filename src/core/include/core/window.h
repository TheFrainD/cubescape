#pragma once

#include <cstdint>
#include <string>

#include "core/dimensions.h"

struct GLFWwindow;
namespace core {
struct WindowSettings {
    Dimensions dimensions;
    std::string title;
    std::uint8_t multisample;
};

enum class WindowError: int {
    kOk = 0,
    kFailedToInitializeGLFW,
    kFailedToCreateGLFWWindow,
    kFailedToInitializeGLAD
};

class Window {
public:
    explicit Window(const WindowSettings &settings);
    ~Window() = default;

    /**
     * @brief Initializes the window.
     */
    WindowError Init();

    /**
     * @brief Deinitializes the window.
     */
    void Deinit();

    Window(const Window &)            = delete;
    Window(Window &&)                 = delete;
    Window &operator=(const Window &) = delete;
    Window &operator=(Window &&)      = delete;

    /**
     * @brief Sets the settings for the window.
     *
     * @param settings The settings to set.
     */
    void SetSettings(const WindowSettings &settings);

    /**
     * @brief Retrieves the settings for the window.
     *
     * @return WindowSettings The settings for the window.
     */
    WindowSettings GetSettings() const;

    /**
     * @brief Retrieves the dimensions of the framebuffer.
     *
     * @return Dimensions The dimensions of the framebuffer.
     */
    Dimensions GetFramebufferDimensions() const;

    /**
     * @brief Checks if the window should close.
     *
     * @return bool True if the window should close, false otherwise.
     */
    bool ShouldClose() const;

    /**
     * @brief Swaps the front and back buffers.
     */
    void SwapBuffers() const;

    /**
     * @brief Polls for and processes events.
     */
    static void PollEvents();

    /**
     * @brief Sets the swap interval for buffer swapping.
     *
     * @param interval The swap interval.
     */
    void SetSwapInterval(int interval) const;

    /**
     * @brief Updates the delta time between frames.
     */
    void UpdateDeltaTime();

    /**
     * @brief Retrieves the delta time between frames.
     */
    float GetDeltaTime() const;

    /**
     * @brief Retrieves the handle for the window.
     * 
     * @return GLFWwindow* The handle for the window.
     */
    GLFWwindow *GetHandle() const;

private:
    GLFWwindow *handle_ {nullptr};
    WindowSettings settings_;

    float last_frame_ {0.0f};
    float delta_time_ {0.0f};
};
}  // namespace core
