# SGDK-Sloped Tiles


A little sampler on how to detect different types of tiles in an SGDK project. 
Contains an asesprite file with a basic level layout, and the assembler script to convert a tilemap into something usable. 

![blastem_20250103_222631](https://github.com/user-attachments/assets/5b87898d-cb5c-4929-a912-1831ecb34e17)

The approach used here is outlined on my blog : https://spacebruce.netlify.app/posts/tilemap/

## To modify the tilemap in asesprite:
- install "COLLISION SCRIPT.lua" into your asesprite scripts folder
- Open the res/imgScene.aseprite file in asesprite
- Modify "Art" layer with new layout. Hide the "Collision" layer and export as PNG. Be careful to only use the first 16 palette entries for graphical assets!
- Modify "Collision" layer with the collision layout, I've buried the tile atlas underground in the level so the entire thing can be indexed by asesprite's tile functionality, you may wish to delete those.
- Hide the "Art" layer and Use the COLLISION SCRIPT to export overwriting E1M1.h in the src folder
