#pragma once

typedef struct noise {
    float (*compute)(struct noise *noise, float x, float y);
} noise_t;