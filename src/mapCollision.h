#pragma once

#include <genesis.h>

extern const uint8_t* LevelCollisionArray;    // Pointer to a const uint8_t array, not a const pointer to a uint8_t array.
extern uint16_t LevelCollisionWidth;
extern uint16_t LevelCollisionHeight;

enum CollisionType
{
    TileBlank = 0, 
    TileSolid = 1, TileSlopeLR = 2, TileSlopeLR2_1 = 3, TileSlopeLR2_2 = 4, TileSlopeLR3_1 = 5,
    TileSlopeLR3_2 = 6, TileSlopeLR3_3 = 7, TileSlopeRL = 8, TileSlopeRL2_1 = 9, TileSlopeRL2_2 = 10,
    TileSlopeRL3_1 = 11, TileSlopeRL3_2 = 12, TileSlopeRL3_3 = 13, TileSolidTopHalf = 14, TileSolidBottomHalf = 15,
    TileSolidLeftHalf = 16, TileSolidRightHalf = 17, 
};

//
void SetMapCollision(const uint8_t* CollisionArray, const uint16_t Width, const uint16_t Height);
uint8_t CheckMapCollision(const int16_t X, const int16_t Y);

static inline __attribute__((always_inline)) uint8_t CheckMapCollisionTileFast(const int16_t TX, const int16_t TY)
{
    // Switch to assembly version if running on a Megadrive
#if defined(__GNUC__) && defined(__m68k__)
    uint8_t result;
    asm volatile
    (
        "move.w %[LevelCollisionWidth], %%d0      \n" // Width
        "mulu.w %[TY], %%d0                      \n"  // TY * Width
        "add.w %[TX], %%d0                       \n"  // + TX
        "move.l %[LevelCollisionArray], %%a0     \n"  // LevelCollisionArray[result]
        "move.b (%%a0, %%d0.w), %[result]        \n"
        : [result] "=r" (result)                 
        : [TX] "r" (TX), [TY] "r" (TY), [LevelCollisionWidth] "m" (LevelCollisionWidth),
          [LevelCollisionArray] "m" (LevelCollisionArray) 
        : "d0", "a0"                             
    );
    return result;
#else
    return LevelCollisionArray[TX + (TY * LevelCollisionWidth)];    
#endif
}