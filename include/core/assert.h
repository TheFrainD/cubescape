#pragma once

#include "core/log.h"

#ifdef ASSERTIONS_ENABLED
// Macro that breaks into the debugger
#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#if defined(__i386__) || defined(__x86_64__)
#define DEBUG_BREAK() __asm__ __volatile__("int $0x03")
#elif defined(__aarch64__) || defined(__arm__)
#define DEBUG_BREAK() __builtin_trap()
#else
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#endif
#else
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#endif

// Assert macro that logs an error message and breaks into the debugger if the expression is false
#define ASSERT(expr)                                                                    \
    if (expr) {                                                                         \
    } else {                                                                            \
        LOG_FATAL("Assertion failed: %s, file %s, line %d", #expr, __FILE__, __LINE__); \
        DEBUG_BREAK();                                                                  \
    }

#define _ASSERT_GLUE(a, b) a##b
#define ASSERT_GLUE(a, b)  _ASSERT_GLUE(a, b)

#else

#define ASSERT(expr)

#endif  // ASSERTIONS_ENABLED

// Static assert macro that fails to compile if the expression is false
#define STATIC_ASSERT(expr) enum { ASSERT_GLUE(static_assert_fail_, __LINE__) = 1 / (int)(!!(expr)) }
