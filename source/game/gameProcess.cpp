#include "gameStatus.h"

void handle_game_loss();
void handle_card_reveal(int card_value);
void handle_game_win();

void update_game() {
    if ((g_state.game_state == GAME_STATE_LOSE || g_state.game_state == GAME_STATE_WIN)) {
        if(!--g_state.freeze) {
            if(g_state.game_state == GAME_STATE_WIN) {
                g_state.total_score += g_state.current_score;
                g_state.current_score = 1;
                g_state.score_changed = 1;
                g_state.total_changed = 1;
            }
            else if(g_state.game_state == GAME_STATE_LOSE) {
                g_state.current_score = 1;
                g_state.score_changed = 1;
            }

            init_game_status();
            g_state.game_state = GAME_STATE_PLAYING;
            g_state.level_changed = 1; // Putting it now to update the graphics later
        }
    }
    if (!g_state.card_turned || g_state.board_status[g_state.cursor_row][g_state.cursor_col]) {
        return;
    }

    int card_value = g_state.board_content[g_state.cursor_row][g_state.cursor_col];
    
    if (card_value == 0) {
        handle_game_loss();
    } else if (card_value > 1) {
        handle_card_reveal(card_value);
    }
}

void handle_game_loss() {
    g_state.game_state = GAME_STATE_LOSE;
    g_state.game_state_changed = 1;
    g_state.current_level = g_state.current_level > MIN_LEVEL ? g_state.current_level - 1 : MIN_LEVEL;
    g_state.freeze = FREEZE_DURATION;
    g_state.reveal_board = 1;
}

void handle_card_reveal(int card_value) {
    g_state.current_score *= card_value;
    g_state.score_changed = 1;
    g_state.cards_to_find--;
    
    if (g_state.cards_to_find == 0) {
        handle_game_win();
    }
}

void handle_game_win() {
    g_state.game_state = GAME_STATE_WIN;
    g_state.game_state_changed = 1;
    g_state.current_level = g_state.current_level < MAX_LEVEL ? g_state.current_level + 1 : MAX_LEVEL;
    g_state.freeze = FREEZE_DURATION; 
    g_state.reveal_board = 1;
}