#include <nds.h>
#include <nf_lib.h>

#include "../game/gameStatus.h"
#include "gameMedia.h"
#include "gameGraphic.h"
#include "gameSfx.h" // SFX Follow graphic events, for that reason sfx is handled here

/**
 * Screens update. 
 */
void render_graphics() {
    NF_SpriteOamSet(0);
    NF_SpriteOamSet(1);
    swiWaitForVBlank();
    oamUpdate(&oamSub);
    oamUpdate(&oamMain);
}

/**
 * Waits for a given number of Vertical Blanks.
 * @param wait The number of Vertical Blanks to wait for.
 */
void waitVB(int wait) {
    for(int i=0; i<wait; i++) {
        render_graphics();
    }  
}

/**
 * Reveals all cards on the board.
 */
void turn_all_cards() {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int id = r*BOARD_SIZE + c;
            NF_SpriteFrame(1, id, g_state.board_content[r][c]);
            NF_SpriteFrame(1, MARKER_SPRITE_BASE+id, 0);
        }
    }
}

/**
 * Moves the cursor sprite to the current position
 */
void handle_cursor_movement() {
    if (g_state.cursor_moved) {
        NF_MoveSprite(1, 50, 
                     g_state.cursor_col * CARD_SIZE + GRID_OFFSET_X, 
                     g_state.cursor_row * CARD_SIZE + GRID_OFFSET_Y);
        g_state.cursor_moved = false;
    }
}

/**
 * Handles the updates of the marker selector sprite based on the current selected marker.
 */
void handle_marker_updates() {
    if (g_state.markers_changed) {
        static int oldMarker = MARKER_NONE;
        if (oldMarker != MARKER_NONE) {
            NF_SpriteFrame(1, MARKER_SELECTOR_SPRITE_BASE + oldMarker, 6 + oldMarker * 2);
        }
        if (g_state.selected_marker != MARKER_NONE) {
            NF_SpriteFrame(1, MARKER_SELECTOR_SPRITE_BASE + g_state.selected_marker, 
                        7 + g_state.selected_marker * 2);
        }
        oldMarker = g_state.selected_marker;

        g_state.markers_changed = false;
    }
}

/**
 * Hadles the updates of the score and record sprites based on the current values.
 */
void handle_score_updates() {
    if (g_state.score_changed) {
        update_score();
        g_state.score_changed = false;
    }
    if (g_state.total_changed) {
        update_record();
        g_state.total_changed = false;
    }
}

/**
 * Shows on screen the win/loss logo and plays the corresponding sound effect.
 */
void handle_level_transitions() {
    if(g_state.game_state_changed) {
        if(g_state.game_state == GAME_STATE_WIN) {
            waitVB(5);
            play_win();
            waitVB(5);
            NF_SpriteFrame(1, 127, 1);
            g_state.game_state_changed = 0;
        }
        if(g_state.game_state == GAME_STATE_LOSE) {
            waitVB(20);
            play_loss();
            waitVB(5);
            NF_SpriteFrame(1, 127, 2);
            g_state.game_state_changed = 0;
        }
    }
}

/**
 * Handles the card interactions, both revealing and marking, based on the current game state.
 */
void handle_card_operations() {
    if (g_state.card_turned) {
        reveal_card();
        g_state.card_turned = false;
    }
    
    if (g_state.card_marked) {
        toggle_card_marker();
        g_state.card_marked = false;
    }
}

/**
 * Reveals the selected card, and if it is a bomb, plays the explosion sound effect.
 */
void reveal_card() {
    if (!g_state.board_status[g_state.cursor_row][g_state.cursor_col]) {
        int card_id = g_state.cursor_row * BOARD_SIZE + g_state.cursor_col;
        NF_SpriteFrame(1, MARKER_SPRITE_BASE + card_id, 0);

        play_turn();
        for(int frame=15; frame <= 19; frame++) {
            NF_SpriteFrame(1, card_id, frame);
            waitVB(2);
        }

        NF_SpriteFrame(1, card_id, g_state.board_content[g_state.cursor_row][g_state.cursor_col]);
        g_state.board_status[g_state.cursor_row][g_state.cursor_col] = true;

        if(!g_state.board_content[g_state.cursor_row][g_state.cursor_col]) {
            play_explosion();
        }
    }
}

/**
 * Toggles the marker on the selected card.
 */
void toggle_card_marker() {
    if (!g_state.board_status[g_state.cursor_row][g_state.cursor_col]) {
        int mask = 1 << g_state.selected_marker;
        int card_id = g_state.cursor_row * BOARD_SIZE + g_state.cursor_col;
        
        g_state.board_markers[g_state.cursor_row][g_state.cursor_col] ^= mask;
        NF_SpriteFrame(1, MARKER_SPRITE_BASE + card_id, g_state.board_markers[g_state.cursor_row][g_state.cursor_col]);
    }
}

/**
 * updates the score sprites.
 */
void update_score() {
    char buf[16];
    play_coin();
    snprintf(buf, sizeof(buf), "%06d", g_state.current_score); 
    for (int i = 0; i < 6; i++) {
        int digit = buf[i] - '0';
        NF_SpriteFrame(0, 100 + i, digit);
    }
}

/**
 * updates the record sprites.
 */
void update_record() {
    char buf[16];
    // Record corrente
    for(int i=0; i<5; i++) {
        play_coin();
    }
    snprintf(buf, sizeof(buf), "%06d", g_state.total_score); 
    for (int i = 0; i < 6; i++) {
        int digit = buf[i] - '0';
        NF_SpriteFrame(0, 106 + i, digit);
    }
}

/**
 * Turns all cards face down and plays the shuffle sound
 */
void hide_all_cards() {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int id = 25+r*BOARD_SIZE + c;
            NF_SpriteFrame(1, id, 0);
        }
    }

    play_shuffle();
    for(int frame=19; frame >= 15; frame--) {
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++) {
                int id = r*BOARD_SIZE + c;
                NF_SpriteFrame(1, id, frame);
            }
        } 
        waitVB(2);
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int id = r*BOARD_SIZE + c;
            NF_SpriteFrame(1, id, 4);
        }
    }
}

/**
 * Reveals all cards on the board with an animation, and plays the shuffle sound effect.
 */
void reveal_board() {
    waitVB(200);  

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int id = 25+r*BOARD_SIZE + c;
            NF_SpriteFrame(1, id, 0);
        }
    }

    play_shuffle();
    for(int frame=15; frame <= 19; frame++) {
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++) {
                if(!g_state.board_status[r][c]) {
                    int id = r*BOARD_SIZE + c;
                    NF_SpriteFrame(1, id, frame);
                }
            }
        } 
        waitVB(2);
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int id = r*BOARD_SIZE + c;
            NF_SpriteFrame(1, id, g_state.board_content[r][c]);
        }
    }
}

