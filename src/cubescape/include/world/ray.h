#pragma once

#include <cglm/struct.h>

#include "world/world.h"

typedef struct {
    vec3s origin;
    vec3s direction;
} ray_t;

/**
 * @brief Casts a ray into the world and returns the block that was hit.
 * 
 * This function casts a ray into the world and returns the block that was hit.
 * 
 * @param world The world to cast the ray into.
 * @param ray The ray to cast.
 * @param max_distance The maximum distance to cast the ray.
 * @param hit_block The block that was hit.
 * @param hit_face The face of the block that was hit.
 *  
 * @return int Returns 1 if a block was hit, 0 otherwise.
 */
int ray_cast(world_t *world, ray_t ray, float max_distance, ivec3s *hit_block, ivec3s *hit_face);
