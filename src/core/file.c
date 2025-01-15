#include "core/file.h"

size_t get_file_size(FILE *fp) {
    long current_pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, current_pos, SEEK_SET);
    return size;
}

int read_file_content(FILE *fp, char *buffer, size_t buffer_size) {
    size_t size = get_file_size(fp);
    if (size > buffer_size) {
        return -1;
    }

    size_t bytes_read = fread(buffer, 1, size, fp);
    buffer[bytes_read] = '\0';

    return bytes_read;
}
