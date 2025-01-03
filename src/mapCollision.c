#include "mapCollision.h"

const int LevelTileSize = 16;

//
const uint8_t* LevelCollisionArray;
uint16_t LevelCollisionWidth;
uint16_t LevelCollisionHeight;

const uint16_t LevelLevelTileSize = 16;
const uint16_t LevelCollisionShift = 4;

void SetMapCollision(const uint8_t* CollisionArray, const uint16_t Width, const uint16_t Height)
{
    LevelCollisionArray = CollisionArray;
    LevelCollisionWidth = Width;
    LevelCollisionHeight = Height;
}

uint8_t CheckMapCollision(const int16_t X, const int16_t Y)
{
    uint16_t tileType = CheckMapCollisionTileFast(X >> LevelCollisionShift, Y >> LevelCollisionShift);
    uint16_t tx, ty;

    // quick return
    switch(tileType)
    {
        case TileBlank: 
            return TileBlank;
        case TileSolid: 
            return TileSolid;
        default:
            tx = X % LevelTileSize;
            ty = Y % LevelTileSize;
        break;
    }

    // Handle slope tiles
    switch(tileType)
    {
        // Ascending slopes
        case TileSlopeLR:       return ty > (LevelTileSize - tx);    break;                      // 1:1 slope
        case TileSlopeLR2_1:    return (2 * ty) > (2 * LevelTileSize - tx);    break;            // 2:1 slope
        case TileSlopeLR2_2:    return (2 * ty) > (2 * LevelTileSize - tx - LevelTileSize);   break;

        case TileSlopeLR3_1:    return (3 * ty) > (3 * LevelTileSize - tx);    break;            // 3:1 slope
        case TileSlopeLR3_2:    return (3 * ty) > (3 * LevelTileSize - tx - LevelTileSize); break;
        case TileSlopeLR3_3:    return (3 * ty) > (3 * LevelTileSize - tx - 2 * LevelTileSize); break;

        // Descending slopes
        case TileSlopeRL:       return ty > tx; break;                                      // 1:1 slope
        case TileSlopeRL2_1:    return (2 * ty) > tx; break;                                // 2:1 slope
        case TileSlopeRL2_2:    return (2 * ty) > (tx + LevelTileSize);    break;
        case TileSlopeRL3_1:    return (3 * ty) > tx; break;                                // 3:1 slope
        case TileSlopeRL3_2:    return (3 * ty) > (tx + LevelTileSize);  break;
        case TileSlopeRL3_3:    return (3 * ty) > (tx + 2 * LevelTileSize);  break;

        // Half blocks
        case TileSolidTopHalf:      return (2 * ty) < LevelTileSize;     break;
        case TileSolidBottomHalf:   return (2 * ty) > LevelTileSize;     break;
        case TileSolidLeftHalf:     return (tx * 2) < LevelTileSize;     break;
        case TileSolidRightHalf:    return (tx * 2) > LevelTileSize;     break;

        default:
            return false;
        break;
    }
}