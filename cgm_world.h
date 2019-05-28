#ifndef CGM_WORLD_H
#define CGM_WORLD_H

#include "SDL.h"
#include "cgm_tilemap.h"
#include "cgm_spriteList.h"

typedef struct pointObj{
    int x, y;
}point;

typedef struct cameraObj{
    int pos_x, pos_y;

    int x, y;
    int w, h;

    int xv, yv;

    int max_x, max_y;
}camera;

typedef struct worldObj{
    int t_w;
    int t_h;

    int cur_layer;
    int num_draw;

    camera view;


    tileMap layers[MAX_LAYERS];
    sprList* spl;

    SDL_Surface* draw_tiles[MAX_DRAW_TILES];
}world;

int wld_init(int tw, int th, int mw, int mh);

int wld_addTiles(char* sheet, int num_tiles, int bor);

/*tile screen coordinate retrieval*/
point wld_getTileCoords(int tx, int ty);

int wld_addSprite(sprite* spr);

void wld_setCameraSpeed(int x_v, int y_v);

void wld_setCameraPos(int p_x, int p_y);

void wld_setCameraDim(int w, int h);

void wld_update();

void wld_render();

void wld_close();

extern world* _wld;

#endif // CGM_WORLD_H
