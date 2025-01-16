#pragma once

#include <stdio.h>
#include <stddef.h>

/**
 * @brief Get the size of a file.
 *
 * This function takes a file pointer and returns the size of the file in bytes.
 *
 * @param fp A pointer to the FILE object that identifies the stream.
 * @return The size of the file in bytes.
 */
size_t get_file_size(FILE *fp);

/**
 * @brief Reads the content of a file into a buffer.
 *
 * This function reads the content of the file pointed to by `fp` and stores it
 * in the provided `buffer` up to a maximum of `buffer_size` bytes.
 *
 * @param fp A pointer to the FILE object that identifies the file to be read.
 * @param buffer A pointer to the buffer where the file content will be stored.
 * @param buffer_size The size of the buffer in bytes.
 * @return The number of bytes read from the file, or -1 if an error occurs.
 */
int read_file_content(FILE *fp, char *buffer, size_t buffer_size);
