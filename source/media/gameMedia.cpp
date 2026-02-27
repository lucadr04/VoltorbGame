#include <nds.h>
#include <nf_lib.h>

#include "../game/gameStatus.h"
#include "gameMedia.h"
#include "font.h"
#include "gameGraphic.h"

MarkerType oldMarker = MARKER_NONE;

// grit-gba x.png -ftB -gb -Gb8 -fh! gTFF00FF
// grit-gba GameTable.png UpperBg.png -ftB -fh! -gb -gu8 -gB8 -pu16 -pS -OGameTable.pal -gTFF00FF

/**
 * Initializes all the sprites for the game.
 */
void init_sprites() {
    // Cards
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int id = r*BOARD_SIZE + c;
            NF_CreateSprite(1, id, 0, 0, GRID_OFFSET_X + c*CARD_SIZE, GRID_OFFSET_Y + r*CARD_SIZE); 
            NF_SpriteLayer(1, id, 2);
            NF_SpriteFrame(1, id, 4);
        }
    }

    // Markers
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int id = MARKER_SPRITE_BASE + r*BOARD_SIZE + c;
            NF_CreateSprite(1, id, 1, 1, GRID_OFFSET_X + c*CARD_SIZE, GRID_OFFSET_Y + r*CARD_SIZE); 
            NF_SpriteLayer(1, id, 1);
            NF_SpriteFrame(1, id, 0);
        }
    }

    // Selection cursor
    NF_CreateSprite(1, 50, 0, 0, GRID_OFFSET_X, GRID_OFFSET_Y); 
    NF_SpriteLayer(1, 50, 1);
    NF_SpriteFrame(1, 50, 5);

    // Marker selection panel
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 2; c++) {
            int id = MARKER_SELECTOR_SPRITE_BASE + r*2 + c;
            NF_CreateSprite(1, id, 0, 0, MARKER_OFFSET_X + c*32, MARKER_OFFSET_Y + r*32); 
            NF_SpriteLayer(1, id, 2);
            NF_SpriteFrame(1, id, 6 + ((r*2+c) * 2));
        }
    }

    // Score and Best score
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 6; c++) {
            int id = SCORE_SPRITE + r*6 + c;
            NF_CreateSprite(0, id, 2, 2, 132 + c*16, 116+ r*32); 
            NF_SpriteLayer(0, id, 0);
            NF_SpriteFrame(0, id, 0);
        }
    }

    // Level
    NF_CreateSprite(0, 126, 2, 2, 128, 178);
    NF_SpriteFrame(0, 126, 1);

    // Win/Loss message
    NF_CreateSprite(1, 127, 3, 3, 192, 100);
    NF_SpriteFrame(1, 127, 0);
}

/**
 * Initializes the game graphics (for the turn)
 */
void init_game_graphics() {
    oldMarker = MARKER_NONE;
    g_state.markers_changed = 1;

    hide_all_cards();

    NF_SpriteFrame(1,127,0);
    NF_SpriteFrame(0,126,g_state.current_level);

    int c1 = 13, c2 = 21, r1 = 5, r2 = 18; // Offsets to draw the numbers on the helperers
    
    NF_Load8bitsBg("GameTable", 0);
    for(int i=0; i<5; i++) {
        draw_digit_to_bg(0, GRID_OFFSET_X+CARD_SIZE*5+c1 , GRID_OFFSET_Y+CARD_SIZE*i+r1, g_state.helperer_row[i][0]/10, 27);
        draw_digit_to_bg(0, GRID_OFFSET_X+CARD_SIZE*5+c2 , GRID_OFFSET_Y+CARD_SIZE*i+r1, g_state.helperer_row[i][0]%10, 27);
        draw_digit_to_bg(0, GRID_OFFSET_X+CARD_SIZE*5+c2 , GRID_OFFSET_Y+CARD_SIZE*i+r2, g_state.helperer_row[i][1], 27);

        draw_digit_to_bg(0, GRID_OFFSET_X+CARD_SIZE*i+c1 , GRID_OFFSET_Y+CARD_SIZE*5+r1, g_state.helperer_col[i][0]/10, 27);
        draw_digit_to_bg(0, GRID_OFFSET_X+CARD_SIZE*i+c2 , GRID_OFFSET_Y+CARD_SIZE*5+r1, g_state.helperer_col[i][0]%10, 27);
        draw_digit_to_bg(0, GRID_OFFSET_X+CARD_SIZE*i+c2 , GRID_OFFSET_Y+CARD_SIZE*5+r2, g_state.helperer_col[i][1], 27);
    }
    NF_Copy8bitsBuffer(1,0,0);
}

/**
 * Updates the graphics based on the current game state.
 */
void update_graphics() {
    if(g_state.level_changed) {
        init_game_graphics();
        g_state.level_changed = 0;
        return;
    }
    handle_cursor_movement();
    handle_marker_updates();
    handle_score_updates();
    handle_level_transitions();
    handle_card_operations();
    
    render_graphics();

    if(g_state.reveal_board) {
        reveal_board();
        g_state.reveal_board = 0;
    }
}