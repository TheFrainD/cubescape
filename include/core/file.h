#pragma once

#include <stdio.h>
#include <stddef.h>

size_t get_file_size(FILE *fp);

int read_file_content(FILE *fp, char *buffer, size_t buffer_size);
