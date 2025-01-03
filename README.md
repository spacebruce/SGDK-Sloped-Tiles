# SGDK Tilemap example

A little sampler on how to detect different types of tiles in an SGDK project. 
Contains an asesprite file with a basic level layout, and the assembler script to convert a tilemap into something usable. 

![blastem_20250103_222631](https://github.com/user-attachments/assets/5b87898d-cb5c-4929-a912-1831ecb34e17)

The general approach used here is outlined on my blog : https://spacebruce.netlify.app/posts/tilemap/

## Adding more types of tiles
- Add a new palette entry to the asesprite file and draw new tile on the Collision layer with that new colour as the top-left pixel
- modify map, gleefully spreading new tile all over
- export using collision script (see below)
- Modify art layer (see below) as required
- Add a new entry to the CollisionType enum in mapCollision.h, making sure the number is +1 from the previous and matches the palette index you just added to the collision data
- If it's a "terrain" collidable type tile, add a handler to the CheckMapCollision function in mapCollision.c (You may find inspiration in my blog article for how to do this)
- If it's a "special" tile with some sort of property you're looking out for, add it to the player handling code in main.c, check out how I added the hurt-block to see how to detect and respond to that sort of collision
- good luck!

## To modify the tilemap in asesprite:
- install "COLLISION SCRIPT.lua" into your asesprite scripts folder
- Open the res/imgScene.aseprite file in asesprite
- Modify "Art" layer with new layout. Hide the "Collision" layer and export as PNG. Be careful to only use the first 16 palette entries for graphical assets!
- Modify "Collision" layer with the collision layout, I've buried the tile atlas underground in the level so the entire thing can be indexed by asesprite's tile functionality, you may wish to delete those.
- Hide the "Art" layer and Use the COLLISION SCRIPT to export overwriting E1M1.h in the src folder
