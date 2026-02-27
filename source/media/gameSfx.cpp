#include <nds.h>
#include <maxmod9.h>
#include "soundbank.h"

/**
 * Plays the sound effect for flipping a card.
 */
void play_turn() {
    mmLoadEffect(SFX_TURN);

    mmEffect(SFX_TURN);
}

/**
 * Plays the sound effect for shuffling the board.
 */
void play_shuffle() {
    mmLoadEffect(SFX_SHUFFLE);
    
    mmEffect(SFX_SHUFFLE);
}

/**
 * Plays the sound effect for obtaining coins.
 */
void play_coin() {
    mmLoadEffect(SFX_COIN);

    mmEffect(SFX_COIN);
}

/**
 * Plays the sound effect for the bomb exploding.
 */
void play_explosion() {
    mmLoadEffect(SFX_EXPLOSION);

    mmEffect(SFX_EXPLOSION);
}

/**
 * Plays the sound effect for losing the game.
 */
void play_loss(){
    mmLoadEffect(SFX_LOSS);

    mmEffect(SFX_LOSS);
}   

/**
 * Plays the sound effect for winning the game.
 */
void play_win(){
    mmLoadEffect(SFX_WIN);

    mmEffect(SFX_WIN);
}