#include "genesis.h"

#include <sprites_res.h>
#include <genesis.h>
#include "mapCollision.h"

// Map
static const uint8_t tilemapCollision[252] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17, 
    16,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17, 
    16,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17, 
    16,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17, 
    16,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17, 
    1,14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,14, 1, 
    16,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17, 
    16,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,17, 
    1,15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15, 1, 
    16,0, 0, 0, 0, 0, 0, 2, 1, 1, 8, 0, 0, 0, 0, 0, 0,17, 
    16,0, 0, 0, 0, 3, 4, 1, 1, 1, 1, 9,10, 0, 0, 0, 0,17, 
    16,0, 5, 6, 7, 1, 1, 1, 1, 1, 1, 1, 1,11,12,13, 0,17, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
};
static const uint16_t mapWidth = 288;
static const uint16_t mapHeight = 224;
static const uint16_t tilemapTileWidth = 288/16;
static const uint16_t tilemapTileHeight = 224/16;

// Game stuff
Sprite* player;
int16_t playerX = 64;
int16_t playerY = 64;
int16_t playerVX = 0;
int16_t playerVY = 0;

//
void gameJoystickHandler(u16 joy, u16 changed, u16 state)
{
    if(joy == JOY_1)
    {
        if(changed & BUTTON_LEFT) playerVX = (-1 * ((state & BUTTON_LEFT) != 0));
        if(changed & BUTTON_RIGHT) playerVX = (1 * ((state & BUTTON_RIGHT) != 0));
        if(changed & BUTTON_UP) playerVY = (-1 * ((state & BUTTON_UP) != 0));
        if(changed & BUTTON_DOWN) playerVY = (1 * ((state & BUTTON_DOWN) != 0));  
    }
};

int main(bool hardReset)
{
    SetMapCollision(tilemapCollision, tilemapTileWidth, tilemapTileHeight);

    VDP_drawImage(BG_B, &imgScene, 0, 0);

    SPR_init();
    player = SPR_addSprite(&sprPlayer, -64, -64, TILE_ATTR(PAL3, true, false, false));
    PAL_setColors(PAL3 * 16, sprPlayer.palette->data, 16, DMA);

    JOY_setEventHandler(gameJoystickHandler);

    int16_t playerXLast = 0;
    int16_t playerYLast = 0;
    while(TRUE)
    {
        SPR_setPosition(player, playerX - 16, playerY - 32);


        // Move player
        if(playerX < 0) playerX = 0;
        if(playerY < 0) playerY = 0;
        if(playerX > mapWidth)  playerX = mapWidth;
        if(playerY > mapHeight)  playerY = mapHeight;
        playerX += playerVX;
        playerY += playerVY;

        // Check for collision against the map
        uint8_t collider = CheckMapCollision(playerX, playerY);

        if(collider == TileBlank)
        {
            SPR_setPalette(player, PAL3);
        }
        else
        {
            SPR_setPalette(player, PAL0);
            playerX = playerXLast;
            playerY = playerYLast;
        }
            
        playerXLast = playerX;
        playerYLast = playerY;

        // SGDK housekeeping
        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}
