#define _XOPEN_SOURCE 700   // ffsll()
#include <stdint.h>
#include <sting.h>          // ffsll()
#include <assert.h>
#include "sdm.h"

static uint64_t map_fm2idx[] = {
// least-significant 4 bits
//   0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, // 0 most
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, // 1 sig-
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, IDX_2A, n____x, n____x, IDX_2D, n____x, n____x, // 2 nif-
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, IDX_3A, n____x, IDX_3C, IDX_3D, IDX_3E, IDX_3F, // 3 icant
n____x, n____x, n____x, n____x, n____x, IDX_45, IDX_46, IDX_47, n____x, n____x, n____x, n____x, n____x, n____x, IDX_4E, IDX_4F, // 4 4
n____x, n____x, n____x, n____x, n____x, IDX_55, IDX_56, n____x, n____x, n____x, n____x, n____x, n____x, n____x, IDX_5E, n____x, // 5 bits
n____x, n____x, n____x, n____x, n____x, n____x, IDX_66, n____x, n____x, n____x, IDX_6A, n____x, IDX_6C, n____x, n____x, n____x, // 6
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, IDX_7D, IDX_7E, n____x, // 7
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, IDX_8C, IDX_8D, IDX_8E, IDX_8F, // 8
n____x, n____x, n____x, n____x, n____x, n____x, n____x, IDX_97, n____x, n____x, IDX_9A, n____x, n____x, n____x, IDX_9E, n____x, // 9
n____x, n____x, n____x, n____x, n____x, IDX_A5, IDX_A6, IDX_A7, IDX_A8, n____x, n____x, n____x, n____x, n____x, n____x, n____x, // a
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, IDX_BF, // b
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, // c
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, // d
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, // e
n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x, n____x};// f

// Converts the FM_df_dm enum value found in config_architecture.h to the bitfield index found in sdm.h.
uint64_t fm2idx( int df_dm )
{
	return map_fm2idx[df_dm];
}

uint64_t idx2ord( uint64_t idx ){
    uint64_t ord ffsll( idx );
    assert( ord );
    return ffsll(idx) - 1;
}

static int map_idx2fm[] = {
    FM_06_2A, FM_06_2D, FM_06_3A, FM_06_3C, FM_06_3D, FM_06_3E, FM_06_3F, FM_06_45,
    FM_06_46, FM_06_47, FM_06_4E, FM_06_4F, FM_06_55, FM_06_56, FM_06_5E, FM_06_66,
    FM_06_6A, FM_06_6C, FM_06_7D, FM_06_7E, FM_06_8C, FM_06_8D, FM_06_8E, FM_06_8F,
    FM_06_97, FM_06_9A, FM_06_9E, FM_06_A5, FM_06_A6, FM_06_A7, FM_06_A8, FM_06_BF
};

// Converts the bitfield index found in sdm.h to the FM_df_dm enum value found in config_architecture.h.
int idx2fm( uint64_t idx ){
    return map_idx2fm[ idx ];
}
