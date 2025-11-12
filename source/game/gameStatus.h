#ifndef STATE_H
#define STATE_H

#include <nds.h>

#define BOARD_SIZE 5
#define INITIAL_BOMBS 5
#define INITIAL_MULTIPLIERS 4
#define MAX_LEVEL 9
#define MIN_LEVEL 1
#define FREEZE_DURATION 100

typedef enum {
    GAME_STATE_PLAYING,
    GAME_STATE_WIN,
    GAME_STATE_LOSE,
    GAME_STATE_PAUSED
} GameState;

// Marker types
typedef enum {
    MARKER_NONE = -1,
    MARKER_BOMB = 0,
    MARKER_ONE = 1,
    MARKER_TWO = 2,
    MARKER_THREE = 3
} MarkerType;

typedef struct {
    // === INPUT STATE === -> those are managed by input
    touchPosition touch;
    u16 keys_held;
    bool keys_active;
    bool touch_active;
    MarkerType selected_marker;
    int cursor_row, cursor_col;

    // === DELAY HANDLERS === -> those are managed by whoever needs them game process
    int keys_input_delay;
    int freeze;

    // === GAME STATE === -> those are managed by game process
    int board_content[BOARD_SIZE][BOARD_SIZE];  // 0=bomb, 1-3=values
    int board_status[BOARD_SIZE][BOARD_SIZE];   // 0=covered, 1=uncovered
    int board_markers[BOARD_SIZE][BOARD_SIZE];  // bitmask of markers
    int helperer_row[BOARD_SIZE][2];
    int helperer_col[BOARD_SIZE][2]; // content of the helperers. 0->tot values, 1->tot bombs

    int current_level;
    int current_score;
    int total_score;
    int cards_to_find;
    GameState game_state;
    
    // === DIRTY FLAGS === -> those are managed by game input and used by game process and game graphics
    bool cursor_moved;
    bool markers_changed;
    bool score_changed;
    bool total_changed;
    bool level_changed;
    bool card_turned; // the card aimed by the cursor is turned. This is both used by process and graphic
    bool card_marked;
    bool game_state_changed;
    bool reveal_board;

    bool input_wait;
    // WHAT IS USED BOTH IN GAME PROCESS AND GAME GRAPHIC WILL BE CHANGED ONLY IN THE LATTER
    
} SharedGameState;

extern SharedGameState g_state;

void init_shared_data(void);
void init_game_status(void);

#endif // STATE_H