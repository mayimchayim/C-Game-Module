#include "cgm_engine.h"
#include "cgm_input.h"
#include "cgm_world.h"

#include "cgm_script.h"

#include <stdio.h>
#include <stdlib.h>

#include "SDL_ttf.h"

/*****
void fill_screen(Uint32 color)
{
    SDL_FillRect(_eng->screen, NULL, color);
}
******/

int main(int argc, char* argv[])
{
    SDL_Event evt;
    sprite sp[4];
    color col;

    TTF_Font* tf;
    SDL_Surface* tfs;
    SDL_Color blue;

    int loop = 1;
    int i;
    int cx, cy;
    int px, py;

    if(eng_init(1024, 768, 0, 1) < 0){
        printf("\nError initializing engine");
        return 1;
    }

    if(TTF_Init() < 0){
        printf("\nError initializing ttf");
        return 1;
    }

    tf = TTF_OpenFont("fonts/Lato-Black.ttf", 40);
    if(tf == NULL){
        printf("\nError loading font");
        return 1;
    }

    /*load script file*/
    cgm_scrLoad("scripts/cmd_fill.txt");

    blue.r = 0;
    blue.g = 0;
    blue.b = 255;
    blue.a = 0;

    col = eng_createColor(255, 255, 255, 0);

    tfs = TTF_RenderText_Solid(tf, "Press a key", blue);

    eng_setFPS(60);

    input_init();
    wld_init(32, 32, 100, 100);

    cx = 0;
    cy = 0;
    px = 0;
    py = 0;

    /*****
    for(i = 0; i < 4; i++){
        spr_init(&sp[i], 34, 22, SPR_ATTR_MULTI_FRAME );
        spr_loadFramesPos(&sp[i], "sheets/spr_1.png", 2, 0, cx, cy);
        cy += 1;

        sp[i].x = px;
        sp[i].y = py;
        px += sp[i].w;

        wld_addSprite(&sp[i]);
    }
    ******/


    printf("\nRGB Int: %0xd", col.value);

    /**GL INIT**/

    while(loop){
        while(SDL_PollEvent(&evt)){
            switch(evt.type){
            case SDL_QUIT:
                printf("\nExiting loop");
                loop = 0;
                break;
            default: break;
            }
        }
        input_update();

        if(key_down(SDL_SCANCODE_ESCAPE)){
            loop = 0;
        }

        if(key_down(SDL_SCANCODE_1)){
            spl_setRemove(_wld->spl, 0);
            tfs = TTF_RenderText_Solid(tf, "(1) Pressed", blue);
        }

        if(key_down(SDL_SCANCODE_2)){
            spl_setRemove(_wld->spl, 1);
            tfs = TTF_RenderText_Solid(tf, "(2) Pressed", blue);
        }

        if(key_down(SDL_SCANCODE_3)){
            spl_setRemove(_wld->spl, 2);
            tfs = TTF_RenderText_Solid(tf, "(3) Pressed", blue);
        }

        if(key_down(SDL_SCANCODE_4)){
            spl_setRemove(_wld->spl, 3);
            tfs = TTF_RenderText_Solid(tf, "(4) Pressed", blue);
        }

        cgm_scrRun();

        for(i = 100; i < 200; i++){
            eng_drawLine(0, i, 1023, i, 0x0000ff00);
        }

        wld_update();

        /**GL RENDER**/
        /* Clear the color and depth buffers. */

        glClear( GL_COLOR_BUFFER_BIT);/* We don’t want to modify the projection matrix. */
        glMatrixMode( GL_MODELVIEW );
        glClearColor(0.0, 0.0, 1.0, 0.0);
        glLoadIdentity( );
        /*
        wld_render();

        SDL_BlitSurface(tfs, NULL, eng_getScreen(), NULL);
        */

        eng_wait();
        eng_flip();
    }

    cgm_scrUnload();

    wld_close();
    input_close();


    SDL_FreeSurface(tfs);
    TTF_Quit();
    eng_close();



    return 0;
}
