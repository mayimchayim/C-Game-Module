#ifndef CGM_SPRITE_H
#define CGM_SPRITE_H

#include "SDL.h"
#include "SDL_image.h"
#include "cgm_engine.h"

#include <stdio.h>

/*sprite states*/
#define SPR_STATE_DEAD      0
#define SPR_STATE_ALIVE     1
#define SPR_STATE_DYING     2
#define SPR_STATE_ANIM_DONE 1

#define SPR_MAX_FRAMES      64
#define SPR_MAX_ANIMATIONS  32

#define SPR_MAX_VARS        64

/*attributes for animation*/
#define SPR_ATTR_SINGLE         1
#define SPR_ATTR_MULTI_FRAME    2
#define SPR_ATTR_MULTI_ANIM     4
#define SPR_ATTR_ANIM_ONE_SHOT  8   /*animation will only perform once*/

typedef struct spriteObj{
    int state;
    int attr;
    int w, h;
    int x, y;
    int xv, yv;

    int remove;


    /*sprite vars*/
    int iVars[SPR_MAX_VARS];
    int fVars[SPR_MAX_VARS];

    int num_frames;
    int cur_frame;
    int cur_anim;
    int anim_index;

    /*TIMING*/
    int anim_count;
    int anim_count_max;

    int* animations[SPR_MAX_ANIMATIONS];


    SDL_Surface* frames[SPR_MAX_FRAMES];


}sprite;

void spr_init(sprite* spr, int w, int h, int attr);

void spr_close(sprite* spr);

void spr_setAnimSpeed(sprite* spr, int as);

void spr_setAnimIndex(sprite* spr, int ai);

int spr_loadFramesPos(sprite* spr, char* img, int num_frames, int bor, int cx, int cy);

int spr_loadFrames(sprite* spr, char* img, int num_frames, int bor);

int spr_loadAnim(sprite* spr, int a_i, int length, int* anim_data);

void spr_update(sprite* spr);

void spr_show(sprite* spr);

#endif // CGM_SPRITE_H
