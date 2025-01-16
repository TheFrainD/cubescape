#include "world/ray.h"

#include <math.h>

int ray_cast(world_t *world, ray_t ray, float max_distance, ivec3s *hit_block, ivec3s *hit_face) {
    ivec3s block_pos = world_to_block(ray.origin);
    vec3s step, t_max, t_delta;

    step = (vec3s) {{ray.direction.x > 0 ? 1 : -1, ray.direction.y > 0 ? 1 : -1, ray.direction.z > 0 ? 1 : -1}};

    t_max = (vec3s) {{(block_pos.x + (ray.direction.x > 0 ? 1 : 0) - ray.origin.x) / ray.direction.x,
                      (block_pos.y + (ray.direction.y > 0 ? 1 : 0) - ray.origin.y) / ray.direction.y,
                      (block_pos.z + (ray.direction.z > 0 ? 1 : 0) - ray.origin.z) / ray.direction.z}};

    t_delta = (vec3s) {{fabsf(1.0f / ray.direction.x), fabsf(1.0f / ray.direction.y), fabsf(1.0f / ray.direction.z)}};

    for (float traveled = 0.0f; traveled < max_distance;) {
        if (t_max.x < t_max.y && t_max.x < t_max.z) {
            traveled = t_max.x;
            t_max.x += t_delta.x;
            block_pos.x += step.x;
            *hit_face = (ivec3s) {-step.x, 0, 0};
        } else if (t_max.y < t_max.z) {
            traveled = t_max.y;
            t_max.y += t_delta.y;
            block_pos.y += step.y;
            *hit_face = (ivec3s) {0, -step.y, 0};
        } else {
            traveled = t_max.z;
            t_max.z += t_delta.z;
            block_pos.z += step.z;
            *hit_face = (ivec3s) {0, 0, -step.z};
        }

        if (world_get_block(world, block_pos) != BLOCK_ID_AIR) {
            *hit_block = block_pos;
            return 1;
        }
    }

    return 0;
}