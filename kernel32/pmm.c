#include <libc.h>
#include <error.h>
#include "mm.h"

#define BLOCK_SIZE 4096 /* same size as VMM block size */
#define BITMAP_BIT_CNT CHAR_BIT

/* conversion macros */
#define MEM_TO_BLOCK_IDX(b) \
    ((b) / BLOCK_SIZE)
#define MEM_TO_BLOCK_OFFSET(b)  \
    ((b) % BLOCK_SIZE )
#define SIZE_KB_TO_BLOCKS(kb)   \
    ((kb) * 1024 / BLOCK_SIZE)
#define SIZE_B_TO_BLOCKS(b) \
    ((b) / BLOCK_SIZE + ((b) % BLOCK_SIZE != 0 ? 1 : 0))

#define BLOCK_IDX_TO_BITMAP_IDX(b_idx)  \
    ((b_idx) / BITMAP_BIT_CNT)
#define BLOCK_IDX_TO_BIT_OFFSET(b_idx)  \
    ((b_idx) % BITMAP_BIT_CNT)
#define BITMAP_IDX_TO_BLOCK_IDX(block_idx)  \
    ((block_idx) * BITMAP_BIT_CNT)

#define BLOCK_TO_MEM(idx)   \
    ((void *)((idx) * BLOCK_SIZE))

enum ALIGN {
    ALIGN_LOW,
    ALIGN_HIGH
};
