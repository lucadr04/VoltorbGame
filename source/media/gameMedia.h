#ifndef MEDIA_H
#define MEDIA_H

#define CARD_SIZE 30
#define GRID_OFFSET_X 8
#define GRID_OFFSET_Y 8
#define MARKER_OFFSET_X 192
#define MARKER_OFFSET_Y 9
#define GRID_SPRITES_BASE 0
#define MARKER_SPRITE_BASE 25
#define MARKER_SELECTOR_SPRITE_BASE 51
#define SCORE_SPRITE 100
#define EFFECT_SPRITE 120

void init_sprites(void); 
void update_graphics(void);
void init_game_graphics(void);

#endif // MEDIA_H


