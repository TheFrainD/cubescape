#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct noise {
    float (*compute)(struct noise *noise, float x, float y);
} noise_t;

#ifdef __cplusplus
}
#endif