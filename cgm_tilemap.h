#ifndef CGM_TILEMAP_H
#define CGM_TILEMAP_H

#include "SDL.h"

#define TILE_TYPE_NONE          0
#define TILE_TYPE_SOLID_UP      1
#define TILE_TYPE_SOLID_RIGHT   2
#define TILE_TYPE_SOLID_DOWN    4
#define TILE_TYPE_SOLID_LEFT    8
#define TILE_TYPE_SOLID_ALL     16

#define MAP_DEFAULT_TILEWIDTH   32
#define MAP_DEFAULT_TILEHEIGHT  32

#define MAX_DRAW_TILES  512
#define MAX_LAYERS      5


typedef struct tileObj{
    int index;
    int type;
}tile;


typedef struct tileMapObj{
    int t_w, t_h;
    int m_w, m_h;

    tile* tiles;
}tileMap;

int tm_init(tileMap* tm, int tw, int th, int mw, int mh);

void tm_close(tileMap* tm);

void tm_setTile(tileMap* tm, int tx, int ty, int tile);
void tm_setTileType(tileMap* tm, int tx, int ty, int type);

tile tm_getTile(tileMap* tm, int tx, int ty);




#endif // CGM_TILEMAP_H
