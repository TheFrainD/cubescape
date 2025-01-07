#include "graphics/window.h"

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "core/log.h"

static GLFWwindow* g_window = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void glfw_error_callback(int error, const char* description) {
    log_error("GLFW error: %s", description);
}

void set_window_title(const char* title) {
    if (!g_window) {
        log_error("'set_window_title' failed: no window created");
        return;
    }
    glfwSetWindowTitle(g_window, title);
}

void get_window_size(int *width, int* height) {
    if (!g_window) {
        log_error("'get_window_size' failed: no window created");
        return;
    }
    glfwGetWindowSize(g_window, width, height);
}

void set_window_size(int width, int height) {
    if (!g_window) {
        log_error("'set_window_size' failed: no window created");
        return;
    }
    glfwSetWindowSize(g_window, width, height);
}

void set_swap_interval(int interval) {
    if (!g_window) {
        log_error("'set_swap_interval' failed: no window created");
        return;
    }
    glfwSwapInterval(interval);
}

void create_window(int width, int height, const char* title) {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        log_fatal("Failed to initialize GLFW");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
#endif

    g_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!g_window) {
        log_fatal("Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);

    glfwMakeContextCurrent(g_window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        log_fatal("Failed to initialize GLAD");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(g_window, &fbWidth, &fbHeight);
    glViewport(0, 0, fbWidth, fbHeight);

    log_info("Window created: %s (%dx%d)", title, width, height);
}


void destroy_window() {
    if (g_window) {
        glfwDestroyWindow(g_window);
        glfwTerminate();
        g_window = NULL;

        log_info("Window destroyed");
    }
}

int window_should_close() {
    return g_window ? glfwWindowShouldClose(g_window) : 1;
}

void swap_buffers() {
    if (!g_window) {
        log_error("'swap_buffers' failed: no window created");
        return;
    }
    glfwSwapBuffers(g_window);
}

void poll_events() {
    glfwPollEvents();
}