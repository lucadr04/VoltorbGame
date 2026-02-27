#include <nf_lib.h>
#include <filesystem.h>
#include <nds.h>
#include <maxmod9.h>

/** 
 * Initialize the two main screens systems and buffers and enables sound 
 */
void init_system(void) {
    nitroFSInit(NULL); 
    NF_SetRootFolder("NITROFS");

    NF_InitSpriteBuffers();
    NF_Init8bitsBgBuffers();

    // Top screen
    NF_Set2D(0, 5);
    NF_InitSpriteSys(0);
    NF_InitBitmapBgSys(0, 0);

    // Bottom screen
    NF_Set2D(1, 5);
    NF_InitSpriteSys(1);
    NF_InitBitmapBgSys(1, 0);

    // Enable sound
    soundEnable();
    mmInitDefault("nitro:/soundbank.bin");
}

/** 
 * Initialize the ram with the necessary backgrounds and the sprites 
 */
void init_ram(void) {
    NF_Load8bitsBg("GameTable", 0);
    NF_Copy8bitsBuffer(1, 0, 0);

    NF_Load8bitsBg("UpperBg", 1);
    NF_Copy8bitsBuffer(0, 0, 1);

    NF_LoadSpriteGfx("BoardTiles", 0, 32, 32);
    NF_LoadSpritePal("BoardTiles", 0);
    NF_VramSpriteGfx(1,0,0,false);
    NF_VramSpritePal(1,0,0);

    NF_LoadSpriteGfx("MarkerTiles", 1, 32, 32);
    NF_LoadSpritePal("MarkerTiles", 1);
    NF_VramSpriteGfx(1,1,1,false);
    NF_VramSpritePal(1,1,1);

    NF_LoadSpriteGfx("Font", 2, 8, 16);
    NF_LoadSpritePal("Font", 2);
    NF_VramSpriteGfx(0,2,2,false);
    NF_VramSpritePal(0,2,2);

    NF_LoadSpriteGfx("WideTiles", 3, 64, 32);
    NF_LoadSpritePal("WideTiles", 3);
    NF_VramSpriteGfx(1,3,3,false);
    NF_VramSpritePal(1,3,3);

    /* sox input.wav --bits 16 --rate 44100 --channels 1 output.raw
    soundEnable();
    NF_InitRawSoundBuffers();
    NF_LoadRawSound("gamecorner", 1, 22050, 0);
    NF_PlayRawSound(1, 127, 64, true, 0);*/
}