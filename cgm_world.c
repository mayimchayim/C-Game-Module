#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"

#include "cgm_world.h"
#include "cgm_engine.h"
#include "cgm_input.h"

world* _wld = NULL;

int wld_init(int tw, int th, int mw, int mh)
{
    int i;

    _wld = (world*)malloc(sizeof(world));
    if(_wld == NULL){
        printf("\nError allocating world");
        return -1;
    }

    for(i = 0; i < MAX_LAYERS; i++){
        if(tm_init(&_wld->layers[i], tw, th, mw, mh) < 0){
            printf("\nError allocating tile layer: %d", i);
            return -1;
        }
    }

    for(i = 0; i < MAX_DRAW_TILES; i++){
        _wld->draw_tiles[i] = NULL;
    }

    /*initialize sprite list*/
    _wld->spl = (sprList*)malloc(sizeof(sprList));
    if(_wld->spl == NULL){
        printf("\nError initializing sprite list");
        return 1;
    }
    spl_init(_wld->spl);
    printf("\nSprite list initalized");


    _wld->t_w = tw;
    _wld->t_h = th;
    _wld->cur_layer = 0;
    _wld->num_draw = 0;

    /*set camera*/
    _wld->view.pos_x = 0;
    _wld->view.pos_y = 0;
    _wld->view.x = 0;
    _wld->view.y = 0;
    _wld->view.w = _eng->scr_w;
    _wld->view.h = _eng->scr_h;
    _wld->view.max_x = (mw * tw) - _wld->view.w;
    _wld->view.max_y = (mh * th) - _wld->view.h;

    _wld->view.xv = 0;
    _wld->view.xv = 0;

    return 1;
}

int wld_addSprite(sprite* spr)
{
    spl_add(_wld->spl, spr);
    printf("\nSprite added");
}

int wld_addTiles(char* sheet, int num_tiles, int bor)
{
    SDL_Surface* sh = NULL;
    int i;
    int x, y;
    SDL_Rect fr;

    x = bor;
    y = bor;

    sh = IMG_Load(sheet);
    if(sh == NULL){
        printf("\nError loading sheet: %s", IMG_GetError());
        return -1;
    }



    for(i = _wld->num_draw; i < (_wld->num_draw + num_tiles); i++){
        fr.x = x;
        fr.y = y;
        fr.w = _wld->t_w;
        fr.h = _wld->t_h;

        _wld->draw_tiles[i] = eng_createSurface(_wld->t_w, _wld->t_h);
        if(_wld->draw_tiles[i] == NULL){
            printf("\nError loading draw tile: %d", i);
            return -1;
        }

        SDL_BlitSurface(sh, &fr, _wld->draw_tiles[i], NULL);
        x += (bor + _wld->t_w);
        if(x >= sh->w){
            x = bor;
            y += (bor + _wld->t_h);
        }
    }

    _wld->num_draw += num_tiles;
}

void wld_setCameraSpeed(int x_v, int y_v)
{
    _wld->view.xv = x_v;
    _wld->view.yv = y_v;
}

void wld_setCameraPos(int p_x, int p_y)
{
    _wld->view.pos_x = p_x;
    _wld->view.pos_y = p_y;
}

void wld_setCameraDim(int w, int h)
{
    _wld->view.w = w;
    _wld->view.h = h;
}

void wld_update()
{
    /*update cam*/
    _wld->view.x += _wld->view.xv;
    _wld->view.y += _wld->view.yv;

    if(_wld->view.x > _wld->view.max_x){
        _wld->view.x = _wld->view.max_x;
    }

    if(_wld->view.y > _wld->view.max_y){
        _wld->view.y = _wld->view.max_y;
    }

    if(_wld->view.x < 0){
        _wld->view.x = 0;
    }
    if(_wld->view.y < 0){
        _wld->view.y = 0;
    }

    /*update sprite list*/
    spl_update(_wld->spl);
}

void wld_render()
{
    int start_col, end_col;/*x*/
    int start_row, end_row;/*y*/
    int off_x, off_y;
    int i, j;
    int x, y;
    int i_layer;
    tile t;

    SDL_Rect draw;
    draw.w = _wld->t_w;
    draw.h = _wld->t_h;

    start_col = floor(_wld->view.x / _wld->t_w);
    end_col = start_col + (_wld->view.w / _wld->t_w);
    start_row = floor(_wld->view.y / _wld->t_h);
    end_row = start_row + (_wld->view.h / _wld->t_w);

    off_x = -_wld->view.x + (start_col * _wld->t_w);
    off_y = -_wld->view.y + (start_row * _wld->t_h);

    for(i_layer = 0; i_layer < MAX_LAYERS; i_layer++){
        for(i = start_col; i <= end_col; i++){
            for(j = start_row; j <= end_row; j++){

                    t = tm_getTile(&_wld->layers[i_layer], i, j);
                    x = (i - start_col) * _wld->t_w + off_x + _wld->view.pos_x;
                    y = (j - start_row) * _wld->t_h + off_y + _wld->view.pos_y;
                    draw.x = x;
                    draw.y = y;

                    SDL_BlitSurface(_wld->draw_tiles[t.index], NULL, eng_getScreen(), &draw);
            }
        }
    }

    /*sprite list*/
    spl_draw(_wld->spl);

}



void wld_close()
{
    int i;

    for(i = 0; i < _wld->num_draw; i++){
        SDL_FreeSurface(_wld->draw_tiles[i]);
        _wld->draw_tiles[i] = NULL;
    }

    for(i = 0; i < MAX_LAYERS; i++){
        tm_close(&_wld->layers[i]);
    }

    spl_delete(_wld->spl);
    free(_wld->spl);
    free(_wld);

}
