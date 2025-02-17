#include "core/window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <utility>

#include "core/assert.h"
#include "core/logger.h"

namespace core {
Window::Window(Context& ctx, const WindowSettings& settings)
    : ctx(ctx), settings_(settings) {}

WindowError Window::Init() {
    glfwSetErrorCallback([](int error, const char* description) {
        LOG_ERROR("GLFW error: {}", description);
    });

    if (!glfwInit()) {
        return WindowError::kFailedToInitializeGLFW;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, settings_.multisample);

    handle_ = glfwCreateWindow(settings_.dimensions.width,
                               settings_.dimensions.height,
                               settings_.title.c_str(), NULL, NULL);
    if (!handle_) {
        glfwTerminate();
        return WindowError::kFailedToCreateGLFWWindow;
    }

    glfwSetFramebufferSizeCallback(handle_,
                                   [](GLFWwindow*, int width, int height) {
                                       glViewport(0, 0, width, height);
                                   });

    glfwSetWindowSizeCallback(handle_, [](GLFWwindow* handle, int width,
                                          int height) {
        Window* window = static_cast<Window*>(glfwGetWindowUserPointer(handle));
        window->settings_.dimensions.width  = width;
        window->settings_.dimensions.height = height;
    });

    glfwSetWindowUserPointer(handle_, this);
    glfwMakeContextCurrent(handle_);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(handle_);
        glfwTerminate();
        return WindowError::kFailedToInitializeGLAD;
    }

    Dimensions framebuffer_dimensions;
    glfwGetFramebufferSize(handle_, &framebuffer_dimensions.x,
                           &framebuffer_dimensions.y);
    glViewport(0, 0, framebuffer_dimensions.x, framebuffer_dimensions.y);

    LOG_INFO("Window initialized: {} ({}x{})", settings_.title,
             settings_.dimensions.width, settings_.dimensions.height);

    return WindowError::kOk;
}

void Window::Deinit() {
    ASSERT(handle_);

    glfwDestroyWindow(handle_);
    glfwTerminate();

    LOG_INFO("Window deinitialized");
}

void Window::SetSettings(const WindowSettings& settings) {
    ASSERT(handle_);

    if (settings_.dimensions != settings.dimensions) {
        glfwSetWindowSize(handle_, settings.dimensions.width,
                          settings.dimensions.height);
    }

    if (settings_.title != settings.title) {
        glfwSetWindowTitle(handle_, settings.title.c_str());
    }

    settings_ = settings;
}

WindowSettings Window::GetSettings() const { return settings_; }

Dimensions Window::GetFramebufferDimensions() const {
    ASSERT(handle_);
    Dimensions dimensions;
    glfwGetFramebufferSize(handle_, &dimensions.x, &dimensions.y);
    return dimensions;
}

bool Window::ShouldClose() const {
    ASSERT(handle_);
    return glfwWindowShouldClose(handle_);
}

void Window::SwapBuffers() const {
    ASSERT(handle_);
    glfwSwapBuffers(handle_);
}

void Window::PollEvents() { glfwPollEvents(); }

void Window::SetSwapInterval(int interval) const {
    ASSERT(handle_);
    glfwSwapInterval(interval);
}

void Window::UpdateDeltaTime() {
    ASSERT(handle_);
    float current_frame = glfwGetTime();
    delta_time_         = current_frame - last_frame_;
    last_frame_         = current_frame;
}

float Window::GetDeltaTime() const {
    ASSERT(handle_);
    return delta_time_;
}

GLFWwindow* Window::GetHandle() const {
    ASSERT(handle_);
    return handle_;
}
}  // namespace core
