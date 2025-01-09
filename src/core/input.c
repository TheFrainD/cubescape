#include "core/input.h"

#include <GLFW/glfw3.h>

#include "core/log.h"

#define CALLBACK_ARRAY_SIZE 64

typedef struct {
    Key keys[KEY_LAST];

    int mouse_buttons[MOUSE_BUTTON_LAST];

    double mouse_x, mouse_y;
    double scroll_x, scroll_y;

    MousePosistionCallback mouse_position_callbacks[CALLBACK_ARRAY_SIZE];
    size_t mouse_position_callback_count;
} InputManager;

static InputManager g_input_manager;
extern GLFWwindow* g_window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= KEY_LAST) {
        return;
    }

    g_input_manager.keys[key] = action != GLFW_RELEASE;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button < 0 || button >= MOUSE_BUTTON_LAST) {
        return;
    }

    g_input_manager.mouse_buttons[button] = action != GLFW_RELEASE;
}

void mouse_position_callback(GLFWwindow* window, double x, double y) {
    g_input_manager.mouse_x = x;
    g_input_manager.mouse_y = y;
    
    for (size_t i = 0; i < g_input_manager.mouse_position_callback_count; i++) {
        g_input_manager.mouse_position_callbacks[i](x, y);
    }
}

void mouse_scroll_callback(GLFWwindow* window, double x, double y) {
    g_input_manager.scroll_x = x;
    g_input_manager.scroll_y = y;
}

void init_input_manager() {
    for (int i = 0; i < KEY_LAST; i++) {
        g_input_manager.keys[i] = GLFW_RELEASE;
    }

    for (int i = 0; i < MOUSE_BUTTON_LAST; i++) {
        g_input_manager.mouse_buttons[i] = GLFW_RELEASE;
    }

    g_input_manager.mouse_x = 0.0;
    g_input_manager.mouse_y = 0.0;
    g_input_manager.scroll_x = 0.0;
    g_input_manager.scroll_y = 0.0;
    g_input_manager.mouse_position_callback_count = 0;

    glfwSetKeyCallback(g_window, key_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetCursorPosCallback(g_window, mouse_position_callback);
    glfwSetScrollCallback(g_window, mouse_scroll_callback);
}

int key_pressed(Key key) {
    return g_input_manager.keys[key] == GLFW_PRESS;
}

int mouse_button_pressed(MouseButton button) {
    return g_input_manager.mouse_buttons[button] == GLFW_PRESS;
}

void get_mouse_position(double *x, double *y) {
    if (x) {
        *x = g_input_manager.mouse_x;
    }
    if (y) {
        *y = g_input_manager.mouse_y;
    }
}

void get_mouse_scroll(double *x, double *y) {
    if (x) {
        *x = g_input_manager.scroll_x;
    }
    if (y) {
        *y = g_input_manager.scroll_y;
    }
}

void add_mouse_position_callback(MousePosistionCallback callback) {
    if (g_input_manager.mouse_position_callback_count < CALLBACK_ARRAY_SIZE) {
        g_input_manager.mouse_position_callbacks[g_input_manager.mouse_position_callback_count++] = callback;
    }
}

void set_cursor_enabled(int enabled) {
    glfwSetInputMode(g_window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}