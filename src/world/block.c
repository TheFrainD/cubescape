#include "world/block.h"

int is_block_opaque(BlockId block) {
    return block != BLOCK_ID_AIR;
}