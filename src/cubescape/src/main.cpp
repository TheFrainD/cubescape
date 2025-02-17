#include "core/logger.h"

int main(int argc, char **argv) {
    core::Logger::Init(EXECUTABLE_NAME ".log");
    LOGGER_DEBUG("Hello, World!");
    return 0;
}
