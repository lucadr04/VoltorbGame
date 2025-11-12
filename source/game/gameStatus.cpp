#include "gameStatus.h"

// The ONE global state instance
SharedGameState g_state;

void init_shared_data(void) {
    // Zero everything
    memset(&g_state, 0, sizeof(SharedGameState));
    
    // Set initial values
    g_state.cursor_row = 0;
    g_state.cursor_col = 0;
    g_state.current_score = 1;
    g_state.total_score = 0;
    g_state.current_level = 1;
    g_state.game_state = GAME_STATE_PLAYING;
    g_state.selected_marker = MARKER_NONE;
}

/** Initialize the game */
void init_game_status(void) {
    int bombs = (int)(g_state.current_level/2) + INITIAL_BOMBS;
    g_state.cards_to_find = (int)(g_state.current_level/2) + INITIAL_MULTIPLIERS;

    for(int i=0; i<BOARD_SIZE; i++) {
        for(int j=0; j<BOARD_SIZE; j++) {
            g_state.board_content[i][j] = 1;
            g_state.board_status[i][j] = 0;
            g_state.board_markers[i][j] = 0;
            g_state.helperer_row[i][0]=5;
            g_state.helperer_row[i][1]=0;
            g_state.helperer_col[i][0]=5;
            g_state.helperer_col[i][1]=0;
        }
    }

    int x; int y;
    int placed = 0;
    int p3 = (g_state.current_level * 100) / 12;

    placed=0;
    while (placed < bombs) {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if (g_state.board_content[x][y] == 1) {
            g_state.board_content[x][y] = 0;
            g_state.helperer_row[x][0] -= 1;
            g_state.helperer_row[x][1] += 1;
            g_state.helperer_col[y][0] -= 1;
            g_state.helperer_col[y][1] += 1;
            placed++;
        }
    }

    placed=0;
    while (placed < g_state.cards_to_find) {
        x = rand() % BOARD_SIZE;
        y = rand() % BOARD_SIZE;
        if (g_state.board_content[x][y] == 1) {
            int val = rand()%100 < p3 ? 3 : 2;
            g_state.board_content[x][y] = val;
            g_state.helperer_row[x][0]+=val-1;
            g_state.helperer_col[y][0]+=val-1;
            placed++;
        }
    }
}