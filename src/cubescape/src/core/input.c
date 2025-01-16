#include "core/input.h"

#include <GLFW/glfw3.h>
#include <cubelog/cubelog.h>

#define CALLBACK_ARRAY_SIZE 64

typedef struct {
    key_code_t keys[KEY_LAST];

    int mouse_buttons[MOUSE_BUTTON_LAST];

    double mouse_x, mouse_y;
    double scroll_x, scroll_y;

    mouse_pos_callback_t mouse_position_callbacks[CALLBACK_ARRAY_SIZE];
    size_t mouse_position_callback_count;

    mouse_scroll_callback_t mouse_scroll_callbacks[CALLBACK_ARRAY_SIZE];
    size_t mouse_scroll_callback_count;

    key_pressed_callback_t key_pressed_callbacks[CALLBACK_ARRAY_SIZE];
    size_t key_pressed_callback_count;

    mouse_button_pressed_callback_t mouse_button_pressed_callbacks[CALLBACK_ARRAY_SIZE];
    size_t mouse_button_pressed_callback_count;
} input_manager_t;

static input_manager_t input_manager;
extern GLFWwindow* g_window;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= KEY_LAST) {
        return;
    }

    input_manager.keys[key] = action != GLFW_RELEASE;
    if (input_manager.keys[key]) {
        for (size_t i = 0; i < input_manager.key_pressed_callback_count; i++) {
            input_manager.key_pressed_callbacks[i](key);
        }
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button < 0 || button >= MOUSE_BUTTON_LAST) {
        return;
    }

    input_manager.mouse_buttons[button] = action != GLFW_RELEASE;
    if (input_manager.mouse_buttons[button]) {
        for (size_t i = 0; i < input_manager.mouse_button_pressed_callback_count; i++) {
            input_manager.mouse_button_pressed_callbacks[i](button);
        }
    }
}

static void mouse_position_callback(GLFWwindow* window, double x, double y) {
    input_manager.mouse_x = x;
    input_manager.mouse_y = y;

    for (size_t i = 0; i < input_manager.mouse_position_callback_count; i++) {
        input_manager.mouse_position_callbacks[i](x, y);
    }
}

static void mouse_scroll_callback(GLFWwindow* window, double x, double y) {
    input_manager.scroll_x = x;
    input_manager.scroll_y = y;

    for (size_t i = 0; i < input_manager.mouse_scroll_callback_count; i++) {
        input_manager.mouse_scroll_callbacks[i](x, y);
    }
}

void input_init() {
    for (int i = 0; i < KEY_LAST; i++) {
        input_manager.keys[i] = GLFW_RELEASE;
    }

    for (int i = 0; i < MOUSE_BUTTON_LAST; i++) {
        input_manager.mouse_buttons[i] = GLFW_RELEASE;
    }

    input_manager.mouse_x                       = 0.0;
    input_manager.mouse_y                       = 0.0;
    input_manager.scroll_x                      = 0.0;
    input_manager.scroll_y                      = 0.0;
    input_manager.mouse_position_callback_count = 0;

    glfwSetKeyCallback(g_window, key_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetCursorPosCallback(g_window, mouse_position_callback);
    glfwSetScrollCallback(g_window, mouse_scroll_callback);
}

int input_key_pressed(key_code_t key) { return input_manager.keys[key] == GLFW_PRESS; }

int input_mouse_button_pressed(mouse_button_code_t button) { return input_manager.mouse_buttons[button] == GLFW_PRESS; }

void input_get_mouse_position(double* x, double* y) {
    if (x) {
        *x = input_manager.mouse_x;
    }
    if (y) {
        *y = input_manager.mouse_y;
    }
}

void input_get_mouse_scroll(double* x, double* y) {
    if (x) {
        *x = input_manager.scroll_x;
    }
    if (y) {
        *y = input_manager.scroll_y;
    }
}

void input_add_mouse_position_callback(mouse_pos_callback_t callback) {
    if (input_manager.mouse_position_callback_count < CALLBACK_ARRAY_SIZE) {
        input_manager.mouse_position_callbacks[input_manager.mouse_position_callback_count++] = callback;
    }
}

void input_add_key_pressed_callback(key_pressed_callback_t callback) {
    if (input_manager.key_pressed_callback_count < CALLBACK_ARRAY_SIZE) {
        input_manager.key_pressed_callbacks[input_manager.key_pressed_callback_count++] = callback;
    }
}

void input_add_mouse_scroll_callback(mouse_scroll_callback_t callback) {
    if (input_manager.mouse_scroll_callback_count < CALLBACK_ARRAY_SIZE) {
        input_manager.mouse_scroll_callbacks[input_manager.mouse_scroll_callback_count++] = callback;
    }
}

void input_add_mouse_button_pressed_callback(mouse_button_pressed_callback_t callback) {
    if (input_manager.mouse_button_pressed_callback_count < CALLBACK_ARRAY_SIZE) {
        input_manager.mouse_button_pressed_callbacks[input_manager.mouse_button_pressed_callback_count++] = callback;
    }
}

void input_set_cursor_enabled(int enabled) {
    glfwSetInputMode(g_window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
