#pragma once

typedef enum {
    BLOCK_ID_AIR = 0,
    BLOCK_ID_COBBLESTONE = 1,
} BlockId;

int is_block_opaque(BlockId block);