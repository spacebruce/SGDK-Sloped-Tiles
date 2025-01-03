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


int main(bool hardReset)
{
    SPR_init();

    // Joystick
    void gameJoystickHandler(u16 joy, u16 changed, u16 state)
    {
        /*
            This looks like a weird way of writing it, but what it's doing is setting the movement Vector to
                -1 if left/up
                0 if neutral
                1 if down/right

            so if LEFT is changed
                vx is -1 if state contains LEFT
                vx is 0 if state doesn't contain LEFT
        */

        if(joy == JOY_1)
        {
            if(changed & BUTTON_LEFT) playerVX = (-1 * ((state & BUTTON_LEFT) != 0));
            if(changed & BUTTON_RIGHT) playerVX = (1 * ((state & BUTTON_RIGHT) != 0));
            if(changed & BUTTON_UP) playerVY = (-1 * ((state & BUTTON_UP) != 0));
            if(changed & BUTTON_DOWN) playerVY = (1 * ((state & BUTTON_DOWN) != 0));  
        }
    };
    JOY_setEventHandler(gameJoystickHandler);

    // Transfer all the map information above into the map handler
    SetMapCollision(tilemapCollision, tilemapTileWidth, tilemapTileHeight);

    // Draw the tilemap to the screen.
    VDP_drawImage(BG_B, &imgScene, 0, 0);

    // Create the player sprite
    player = SPR_addSprite(&sprPlayer, -64, -64, TILE_ATTR(PAL3, true, false, false));
    PAL_setColors(PAL3 * 16, sprPlayer.palette->data, 16, DMA);

    // Last position player was in
    int16_t playerXLast = 0;
    int16_t playerYLast = 0;

    // game loop de loop
    while(TRUE)
    {
        // Move player
        if(playerX < 0) playerX = 0;
        if(playerY < 0) playerY = 0;
        if(playerX > mapWidth)  playerX = mapWidth;
        if(playerY > mapHeight)  playerY = mapHeight;
        playerX += playerVX;
        playerY += playerVY;

        SPR_setPosition(player, playerX - 15, playerY - 31);

        // Check for collision against the map
        uint8_t collider = CheckMapCollision(playerX, playerY);

        if(collider == TileBlank)   
        {
            //If hit empty air, use default player palette
            SPR_setPalette(player, PAL3);
        }
        else
        {
            // Signal a hit by flashing to an alternate palette
            SPR_setPalette(player, PAL0);
            // Bounce back a step to last good position
            playerX = playerXLast;
            playerY = playerYLast;
        }
            
        // Remember last position
        playerXLast = playerX;
        playerYLast = playerY;

        // SGDK housekeeping
        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}
