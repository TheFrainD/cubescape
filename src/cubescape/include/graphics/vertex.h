#pragma once

#include <cglm/struct.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    vec3s position;
    vec2s uv;
    vec3s tint;
} vertex_t;

#ifdef __cplusplus
}
#endif
