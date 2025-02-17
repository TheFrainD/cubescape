#include "core/input/input_manager.h"

#include <GLFW/glfw3.h>

#include "core/event/key_event.h"
#include "core/event/mouse_event.h"
#include "core/window.h"

namespace core {
math::Point2D InputManager::mouse_prev_position_;
math::Direction2D InputManager::mouse_delta_ = {0, 0};

void InputManager::Init() {
    mouse_prev_position_ = GetMousePosition();

    GLFWwindow *handle = glfwGetCurrentContext();
    glfwSetKeyCallback(
        handle, [](GLFWwindow *handle, int key, int, int action, int) {
            Window *window =
                static_cast<Window *>(glfwGetWindowUserPointer(handle));
            EventBus &event_bus = window->ctx.event_bus;

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event;
                    event.key = static_cast<Key>(key);
                    event_bus.Trigger(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event;
                    event.key = static_cast<Key>(key);
                    event_bus.Trigger(event);
                    break;
                }
                default:
                    break;
            }
        });

    glfwSetMouseButtonCallback(
        handle, [](GLFWwindow *handle, int button, int action, int) {
            Window *window =
                static_cast<Window *>(glfwGetWindowUserPointer(handle));
            EventBus &event_bus = window->ctx.event_bus;

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event;
                    event.button = static_cast<MouseButton>(button);
                    event_bus.Trigger(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event;
                    event.button = static_cast<MouseButton>(button);
                    event_bus.Trigger(event);
                    break;
                }
                default:
                    break;
            }
        });

    glfwSetCursorPosCallback(
        handle, [](GLFWwindow *handle, double x, double y) {
            Window *window =
                static_cast<Window *>(glfwGetWindowUserPointer(handle));
            EventBus &event_bus = window->ctx.event_bus;

            math::Direction2D delta = {x - mouse_prev_position_.x,
                                       y - mouse_prev_position_.y};
            mouse_prev_position_    = {x, y};

            MouseMovedEvent event;
            event.position = {x, y};
            event.delta    = delta;
            event_bus.Trigger(event);
        });
}

bool InputManager::KeyPressed(Key key) {
    GLFWwindow *handle = glfwGetCurrentContext();
    return glfwGetKey(handle, static_cast<int>(key)) == GLFW_PRESS;
}

bool InputManager::MouseButtonPressed(MouseButton button) {
    GLFWwindow *handle = glfwGetCurrentContext();
    return glfwGetMouseButton(handle, static_cast<int>(button)) == GLFW_PRESS;
}

math::Point2D InputManager::GetMousePosition() {
    double x, y;
    GLFWwindow *handle = glfwGetCurrentContext();
    glfwGetCursorPos(handle, &x, &y);
    return {x, y};
}

math::Direction2D InputManager::GetMouseDelta() { return mouse_delta_; }
}  // namespace core
