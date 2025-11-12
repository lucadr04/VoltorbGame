#include <nds.h>
#include <maxmod9.h>
#include "soundbank.h"

void play_game_ost() {
    mmLoad(MOD_CASINO);
    mmSetModuleVolume(218);
    mmStart(MOD_CASINO, MM_PLAY_LOOP);
}

void pause_ost() {
    mmPause();
}

void resume_ost() {
    mmResume();
}