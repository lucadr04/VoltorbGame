#include <nds.h>
#include <maxmod9.h>
#include "soundbank.h"

/**
 * Plays the main game ost
 */
void play_game_ost() {
    mmLoad(MOD_CASINO);
    mmSetModuleVolume(218);
    mmStart(MOD_CASINO, MM_PLAY_LOOP);
}

/**
 * Pauses the main game ost
 */
void pause_ost() {
    mmPause();
}

/**
 * Resumes the main game ost
 */
void resume_ost() {
    mmResume();
}