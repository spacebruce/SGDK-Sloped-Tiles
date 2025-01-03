#include "genesis.h"

#include <sprites_res.h>
#include <genesis.h>
#include "mapCollision.h"

// Map data in this file : 
#include "E1M1.h"

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
    SetMapCollision(E1M1Collision, E1M1TileWidth, E1M1TileHeight);

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
        playerX += playerVX;
        playerY += playerVY;

        // Keep within level bounds
        if(playerX < 0) playerX = 0;
        if(playerY < 0) playerY = 0;
        if(playerX > E1M1MapWidth)  playerX = E1M1MapWidth;
        if(playerY > E1M1MapHeight)  playerY = E1M1MapHeight;
        
        SPR_setPosition(player, playerX - 16, playerY - 31);

        // Check for collision against the map
        uint8_t collider = CheckMapCollision(playerX, playerY);

        // Jump-through platform handling
        if(collider == TileJumpthrough) 
        {
            if(playerVY < 0)    //if player is moving UP, pretend there was nothing there
                collider = TileBlank;
            else                //if player is moving DOWN, pretend it's a solid block
                collider = TileSolid;
        }
        
        //
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
