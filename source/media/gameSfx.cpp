#include <nds.h>
#include <maxmod9.h>
#include "soundbank.h"

void play_turn() {
    mmLoadEffect(SFX_TURN);

    mmEffect(SFX_TURN);
}

void play_shuffle() {
    mmLoadEffect(SFX_SHUFFLE);
    
    mmEffect(SFX_SHUFFLE);
}


void play_coin() {
    mmLoadEffect(SFX_COIN);

    mmEffect(SFX_COIN);
}

void play_explosion() {
    mmLoadEffect(SFX_EXPLOSION);

    mmEffect(SFX_EXPLOSION);
}

void play_loss(){
    mmLoadEffect(SFX_LOSS);

    mmEffect(SFX_LOSS);
}   

void play_win(){
    mmLoadEffect(SFX_WIN);

    mmEffect(SFX_WIN);
}