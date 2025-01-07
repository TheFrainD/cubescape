#pragma once

void create_window(int width, int height, const char* title);

void destroy_window();

void set_window_title(const char* title);

void get_window_size(int *width, int* height);

void set_window_size(int width, int height);

int window_should_close();

void swap_buffers();

void poll_events();

void set_swap_interval(int interval);