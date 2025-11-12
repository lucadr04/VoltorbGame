#include <nds.h>
#include <nf_lib.h>

#include "../game/gameStatus.h"
#include "../media/gameMedia.h"

void handle_dpad(void);
void handle_touch(void);

typedef enum {
    OUT,
    BOARD,
    MARKERS
} PressZone;

int r; 
int c;
PressZone pressZone = OUT;

void handle_input(void) {
    if(g_state.freeze) return; // Make unfreezing onclick
    scanKeys();
    g_state.keys_held = keysHeld();
    touchRead(&g_state.touch);

    handle_touch();
    handle_dpad();
}

void handle_dpad() {
    if (!g_state.keys_held) {
        g_state.keys_input_delay = 0;
        return;
    }
    if(g_state.keys_input_delay) {
        g_state.keys_input_delay--;
        return;
    }

    g_state.keys_input_delay = 8;

    if (g_state.keys_held & KEY_UP && g_state.cursor_row > 0) {
        g_state.cursor_row--; 
        g_state.cursor_moved = true;
    } else if (g_state.keys_held & KEY_RIGHT && g_state.cursor_col < BOARD_SIZE-1) {
        g_state.cursor_col++; 
        g_state.cursor_moved = true;
    } else if (g_state.keys_held & KEY_DOWN && g_state.cursor_row < BOARD_SIZE-1) {
        g_state.cursor_row++;
        g_state.cursor_moved = true;
    } else if (g_state.keys_held & KEY_LEFT && g_state.cursor_col > 0) {
        g_state.cursor_col--; 
        g_state.cursor_moved = true;
    }

    if (g_state.keys_held & KEY_A) {
        if(g_state.selected_marker == MARKER_NONE) {
            g_state.card_turned = true;
        } else {
            g_state.card_marked = true;
        }
    } else if (g_state.keys_held & KEY_B) {
        MarkerType m = (MarkerType)(((g_state.selected_marker + 2) % 5)-1);
        g_state.markers_changed = true;
        g_state.selected_marker = m;
    }
}

void handle_touch() {
    if (!(g_state.keys_held & KEY_TOUCH)) {
        if(g_state.touch_active) {
            if(pressZone == BOARD) {
                if(g_state.selected_marker == MARKER_NONE) {
                    g_state.card_turned = true;
                } else {
                    g_state.card_marked = true;
                }
            } else if (pressZone == MARKERS) {
                MarkerType m = (MarkerType)(r*2+c);
                g_state.markers_changed = 1;
                g_state.selected_marker = g_state.selected_marker == m ? MARKER_NONE : m;
            } 
            g_state.touch_active = 0;
        }
        return;
    }

    g_state.touch_active = 1;

    if (g_state.touch.px >= 0 && g_state.touch.px < GRID_OFFSET_X + CARD_SIZE * BOARD_SIZE && g_state.touch.py >= 0 && g_state.touch.py < GRID_OFFSET_Y + CARD_SIZE * BOARD_SIZE) {
        c = (g_state.touch.px - GRID_OFFSET_X) / CARD_SIZE;
        r = (g_state.touch.py - GRID_OFFSET_Y) / CARD_SIZE;
        pressZone = BOARD;

        if(g_state.cursor_col != c || g_state.cursor_row != r) {
            g_state.cursor_col = c;
            g_state.cursor_row = r;
            g_state.cursor_moved = 1;
            g_state.touch_active = 1;
        }
        return;
    }

    if (g_state.touch.px >= MARKER_OFFSET_X+4 && g_state.touch.px < MARKER_OFFSET_X+28*2 && g_state.touch.py >= MARKER_OFFSET_Y+4 && g_state.touch.py < MARKER_OFFSET_Y+28*2) {
        c = (g_state.touch.px - MARKER_OFFSET_X) / 32; //marker board size
        r = (g_state.touch.py - MARKER_OFFSET_Y) / 32;
        pressZone = MARKERS;

        g_state.touch_active = 1;
        return;
    }

    pressZone = OUT;
}