#include <time.h>
#include <stdlib.h>

#include "engine/engineInitializer.h"
#include "game/gameStatus.h"
#include "media/gameMedia.h"
#include "media/gameOst.h"
#include "input/gameInput.h"
#include "game/gameProcess.h"

int main(int argc, char **argv) {
    srand(time(NULL));

    init_system();
    init_ram();
    init_shared_data();
    init_sprites();

    init_game_status();
    init_game_graphics();

    play_game_ost();

    while(1) {
        handle_input();
        update_game();
        update_graphics();
    }
}