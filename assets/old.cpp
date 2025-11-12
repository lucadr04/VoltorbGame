// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2024

#include <stdio.h>
#include <time.h>

#include <nds.h>
#include <filesystem.h>

#include <nf_lib.h>

#include "font.h"
#include "board.h"

PrintConsole topScreen;

void init_system() {
    // File system initialization with nitrofs
    // To work the R4 has to support DLDI/argv
    // Or to get patched
    nitroFSInit(NULL); 
    NF_SetRootFolder("NITROFS");

    // Enabling the buffers and the systems to use sprites and 8bit background
    NF_InitSpriteBuffers();
    NF_Init8bitsBgBuffers();

    // Initialize 2D graphical engine on the top screen and the necessary to load sprites and bg
    NF_Set2D(0, 5);
    NF_InitSpriteSys(0);
    NF_InitBitmapBgSys(0, 0);

    // Initialize 2D graphical engine on the bottom screen and the necessary to load sprites and bg
    NF_Set2D(1, 5);
    NF_InitSpriteSys(1);
    NF_InitBitmapBgSys(1, 0);
}

void init_graphic() {
    // Load the background for the game onto vram
    // grit-gba Table.png -ftB -gB8 -fh! -gb -gTFF00FF
    // Magenta for transparency!
    NF_Load8bitsBg("GameTable", 0);
    NF_Copy8bitsBuffer(1, 0, 0);

    NF_Load8bitsBg("UpperBg", 1);
    NF_Copy8bitsBuffer(0, 0, 1);

    // Load the necessary tilesets onto vram. Every tile is 32x32
    // BoardTiles (0) contains the cards: 0->bomb / 1 / 2 / 3 / 4->card / 5->selected / 6->mark (unused)
    // MarkerTiles (1) contains the markers, made to be calculated with OR: From left to right: B/3/2/1
    NF_LoadSpriteGfx("BoardTiles", 0, 32, 32);
    NF_LoadSpritePal("BoardTiles", 0);
    NF_VramSpriteGfx(1,0,0,false);
    NF_VramSpritePal(1,0,0);
    NF_LoadSpriteGfx("MarkerTiles", 1, 32, 32);
    NF_LoadSpritePal("MarkerTiles", 1);
    NF_VramSpriteGfx(1,1,1,false);
    NF_VramSpritePal(1,1,1);
}

void init_sprites() {
    // Create the cards sprites (id from 0 to 24), from dx to sx then from top to bottom
    for (int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            NF_CreateSprite(1, i*5+j, 0, 0, CARD_OFFSET + (j * CARD_SIZE), CARD_OFFSET + (i * CARD_SIZE)); 
            NF_SpriteLayer(1, i*5+j, 2);
        }
    }

    // Create the marker sprites (id from 25 to 49), from dx to sx then from top to bottom
    for (int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            NF_CreateSprite(1, 25+i*5+j, 1, 1, CARD_OFFSET + (j * CARD_SIZE), CARD_OFFSET + (i * CARD_SIZE)); 
            NF_SpriteLayer(1, 25+i*5+j, 1);
            NF_SpriteFrame(1, 25+i*5+j, 0);
        }
    }

    // Create the overlay sprite that will point to the current selected card (id 50)
    NF_CreateSprite(1, 50, 0, 0, CARD_OFFSET, CARD_OFFSET); 
    NF_SpriteLayer(1, 50, 1);
    NF_SpriteFrame(1, 50, 5);

    // Create the 4 sprites that will be used to select the mark to apply (sprites start from 6) (id 51-54)
    for (int i = 0; i < 2; i++) {
        for(int j = 0; j < 2; j++) {
            NF_CreateSprite(1, 51+i*2+j, 0, 0, 192 + (j * 32), 8 + (i * 32)); 
            NF_SpriteLayer(1, 51+i*2+j, 2);
            NF_SpriteFrame(1, 51+i*2+j, 6+((i*2+j)*2));
            printf("%d ", 51+i*2+j);
        }
    }
}

int main(int argc, char **argv) {
    //=====INITIALIZATION==============
    touchPosition touchPos;
    int pressing = 0;
    int delay = 0;
    int marking = -1;
    
    srand(time(NULL));
    
    init_system();
    init_graphic();
    init_sprites();
    
    init_board();
    NF_Copy8bitsBuffer(1,0,0);

    //===CICLO=DI=GIOCO===============
    while (1) {
        scanKeys();
        // int keys = keysDown();
        touchRead(&touchPos);
        int keys_held = keysHeld();

        if (keys_held & KEY_TOUCH) {
            int c = (touchPos.px - CARD_OFFSET);
            if (c > 0) c = c / CARD_SIZE;
            int r = (touchPos.py - CARD_OFFSET);
            if (r > 0) r = r / CARD_SIZE;

            if(c >= 0 && c <= 4 && r >= 0 && r <= 4) {
                newc = c;
                newr = r;
                pressing = 1;
            } else {
                pressing = -1;
                c = (touchPos.px - 192);
                if (c > 0) c = c / 32;
                r = (touchPos.py - 8);
                if (r > 0) r = r / 32;

                if(c >= 0 && c <= 1 && r >= 0 && r <= 1 && !delay) {
                    if(marking == -1) {
                        marking = (r*2+c);
                        NF_SpriteFrame(1, 51+marking, 7+marking*2);
                    } else if(marking == (r*2+c)) {
                        NF_SpriteFrame(1, 51+marking, 6+marking*2);
                        marking = -1;
                    } else {
                        NF_SpriteFrame(1, 51+marking, 6+marking*2);
                        marking = (r*2+c);
                        NF_SpriteFrame(1, 51+marking, 7+marking*2);
                    }
                }
                delay = 5;
            }
        } 
        
        if(!keys_held) delay = 0;
        if(!delay) {
            if (keys_held & KEY_UP && newr > 0) {
                newr--; delay = 15;
            } else if (keys_held & KEY_RIGHT && newc < 4) {
                newc++; delay = 15;
            } else if (keys_held & KEY_DOWN && newr < 4) {
                newr++; delay = 15;
            } else if (keys_held & KEY_LEFT && newc > 0) {  
                newc--; delay = 15;
            }
        }

        NF_MoveSprite(1, 50, newc*CARD_SIZE+CARD_OFFSET, newr*CARD_SIZE+CARD_OFFSET);
        
        if((keys_held & KEY_A && !delay) || (pressing==1 && !(keys_held & KEY_TOUCH))) {
            if(board_status[newr][newc] == 0) {
                if(marking == -1) { 
                    NF_SpriteFrame(1, newr*5 + newc, board_content[newr][newc]);
                    NF_SpriteFrame(1, 25 + newr*5 + newc, 0);
                    board_status[newr][newc] = 1;

                    int val = board_content[newr][newc];
                    int new_score = current_score* val;
                    if(val != 0) {
                        printf("Score=%d*%d=%d\n", current_score, val, new_score);
                        current_score = new_score;
                        if(val>1) {
                            to_find--;
                            if(!to_find) {
                                total_score += current_score;
                                printf("YOU WON GGWP!\n total_score=%d\n", total_score);
                                current_score = 1;
                                current_level++;
                                NF_Load8bitsBg("GameTable", 0);
                                init_board();
                                NF_Copy8bitsBuffer(1, 0, 0);
                            }
                        }
                    } else {
                        printf("YOU LOST HAHAAHAHAH!\n");
                        current_score = 1;
                        NF_Load8bitsBg("GameTable", 0);
                        init_board();
                        NF_Copy8bitsBuffer(1, 0, 0);
                    }
                } else {
                    int mask = 1 << (marking);
                    printf("\n (%d), %d", marking, mask);
                    if(board_markers[newr][newc] & mask) {
                        board_markers[newr][newc] -= mask;
                        NF_SpriteFrame(1, 25+newr*5+newc, board_markers[newr][newc]);
                    } else {
                        board_markers[newr][newc] += mask;
                        NF_SpriteFrame(1, 25+newr*5+newc, board_markers[newr][newc]);
                    }
                    printf("[%d][%d]=%d", newr, newc, board_markers[newr][newc]);
                }
                pressing = false;
            }
        }

        NF_SpriteOamSet(1);

        swiWaitForVBlank();

        oamUpdate(&oamSub);

        if(delay > 0) delay--;
    }

    return 0;
}

