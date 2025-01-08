#include "core/input.h"

#include <GLFW/glfw3.h>

#include "core/log.h"

typedef struct {
    Key keys[KEY_LAST];
    int mouse_buttons[MOUSE_BUTTON_LAST];
    double mouse_x, mouse_y;
    double scroll_x, scroll_y;
} InputManager;

static InputManager input_manager;
extern GLFWwindow* g_window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= KEY_LAST) {
        return;
    }

    input_manager.keys[key] = action;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button < 0 || button >= MOUSE_BUTTON_LAST) {
        return;
    }

    input_manager.mouse_buttons[button] = action;
}

void mouse_position_callback(GLFWwindow* window, double x, double y) {
    input_manager.mouse_x = x;
    input_manager.mouse_y = y;
}

void mouse_scroll_callback(GLFWwindow* window, double x, double y) {
    input_manager.scroll_x = x;
    input_manager.scroll_y = y;
}

void init_input_manager() {
    for (int i = 0; i < KEY_LAST; i++) {
        input_manager.keys[i] = GLFW_RELEASE;
    }

    for (int i = 0; i < MOUSE_BUTTON_LAST; i++) {
        input_manager.mouse_buttons[i] = GLFW_RELEASE;
    }

    input_manager.mouse_x = 0.0;
    input_manager.mouse_y = 0.0;
    input_manager.scroll_x = 0.0;
    input_manager.scroll_y = 0.0;

    glfwSetKeyCallback(g_window, key_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetCursorPosCallback(g_window, mouse_position_callback);
    glfwSetScrollCallback(g_window, mouse_scroll_callback);
}

int key_pressed(Key key) {
    return input_manager.keys[key] == GLFW_PRESS;
}

int mouse_button_pressed(MouseButton button) {
    return input_manager.mouse_buttons[button] == GLFW_PRESS;
}

void get_mouse_position(double *x, double *y) {
    if (x) {
        *x = input_manager.mouse_x;
    }
    if (y) {
        *y = input_manager.mouse_y;
    }
}

void get_mouse_scroll(double *x, double *y) {
    if (x) {
        *x = input_manager.scroll_x;
    }
    if (y) {
        *y = input_manager.scroll_y;
    }
}
