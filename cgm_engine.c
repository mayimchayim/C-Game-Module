#include <stdio.h>
#include <stdlib.h>

#include "cgm_engine.h"

#include "SDL_image.h"



/*engine globals*/
engine* _eng = NULL;


/*functions*/
int eng_init(int w, int h, int full, int ogl)
{
    int flag; /*for full*/

    /*GL disabled by default*/
    _eng->gl_enabled = 0;

    /*allocate engine*/
    _eng = (engine*)malloc(sizeof(engine));
    if(_eng == NULL){
        printf("\nError allocating engine");
        return -1;
    }

    _eng->scr_w = w;
    _eng->scr_h = h;

    if(full){
        _eng->fullscreen = 1;
        flag = SDL_WINDOW_FULLSCREEN;
    }
    else{
        _eng->fullscreen = 0;
        flag = SDL_WINDOW_SHOWN; /*windowed mode if 0*/
    }

    if(ogl){
        printf("\nGL enabled");
        _eng->gl_enabled = 1;
        flag = SDL_WINDOW_OPENGL;
    }
    else{
        _eng->gl_enabled = 0;
    }

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0){
        printf("\neng_init - Unable to initialize SDL");
        return -1; /*error*/
    }

    /*initialize image extension*/
    if(IMG_Init(IMG_INIT_PNG) < 0){
        printf("\neng_init - Unable to load Image library");
        return -1;
        /*TODO - put flag in engine structure to signal extension support*/
    }

    _eng->window = SDL_CreateWindow("cgAmE", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, _eng->scr_w, _eng->scr_h, flag);

    if(_eng->window == NULL){
        printf("\neng_init - Window not createdL %s", SDL_GetError());
        return -1;
    }

    if(ogl){
        _eng->glc = SDL_GL_CreateContext(_eng->window);
        if(_eng->glc == NULL){
            printf("\nError creating GL context");
            return -1;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
        SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
        SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
        SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

        SDL_GL_SetSwapInterval(1);
    }

    /*get screen surface from window*/
    _eng->screen = SDL_GetWindowSurface(_eng->window);
    if(_eng->screen == NULL){
        printf("\neng_init - Surface not retrieved: %s", SDL_GetError());
        return -1;
    }

    /*SUCCESS*/
    return 1;

}

void eng_close()
{
    if(_eng->glc){
        SDL_GL_DeleteContext(_eng->glc);
    }

    /*close window and exit*/
    if(_eng->window){
        SDL_DestroyWindow(_eng->window);
    }

    free(_eng);

    IMG_Quit();

    SDL_Quit();
}

/*timing*/
void eng_setFPS(int fps)
{
    _eng->fps = fps;
    _eng->tick_count = (1000) / _eng->fps;

    _eng->cur_tick = SDL_GetTicks();
    _eng->next_tick = _eng->cur_tick + _eng->tick_count;
}

int eng_wait()
{
    _eng->cur_tick = SDL_GetTicks();

    if(_eng->cur_tick >= _eng->next_tick){
        _eng->next_tick = _eng->cur_tick + _eng->tick_count;
        return 0;
    }
    else{
        SDL_Delay(_eng->next_tick - _eng->cur_tick);

        return _eng->next_tick - _eng->cur_tick;
    }
}


SDL_Surface* eng_createSurface(int w, int h)
{
    SDL_PixelFormat fmt = *(_eng->screen->format);

    return SDL_CreateRGBSurface(0, w, h, fmt.BitsPerPixel,
                                fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
}


SDL_Surface* eng_getScreen()
{
    return _eng->screen;
}

void eng_fillScreen(Uint32 color)
{
    SDL_FillRect(_eng->screen, NULL, color);
}

void eng_drawPoint(int x, int y, Uint32 color)
{
    Uint8* p;
    SDL_Surface* screen = eng_getScreen();

    p = (Uint8*)screen->pixels + y * screen->pitch + x * 4;
    SDL_LockSurface(screen);
    *(Uint32*)p = color;
    SDL_UnlockSurface(screen);
}

void eng_drawLine(int x0, int y0, int x1, int y1, Uint32 color)
{
    int dx, dy; /*determines right/left(dx) and up/down(dy)*/
    int x_inc, y_inc; /*increment in pixels next point in line*/
    int x, y; /*plot coordinates*/
    int dx2, dy2; /*dx and dy * 2*/
    int error;
    int i; /*for looping*/

    Uint8 *p;
    SDL_Surface* screen = eng_getScreen();


    dx = x1 - x0; /*if Positive, line moves right, else left*/
    dy = y1 - y0; /*if positive line moves down, else up*/

    dx2 = dx * 2;
    dy2 = dy * 2;

    if(dx >= 0){
        x_inc = 1;
    }
    else{
        x_inc = -1;
        dx = -dx;
    }

    if(dy >= 0){
        y_inc = 1;
    }
    else{
        y_inc = -1;
        dy = -dy;
    }

    x = x0;
    y = y0;

    if(dx > dy){
        error = dy2 - dx;
    }else{
        error = dx2 - dy;
    }

    /*enter plotting*/
    SDL_LockSurface(screen);



    if(dx > dy){
        /*x loop movement*/
        for(i = 0; i < dx; i++){
            /*get pixel at current x and y*/
            p = (Uint8*)screen->pixels + y * screen->pitch + x * 4;

            *(Uint32*)p = color;

            if(error >= 0){
                error -= dx2;
                y += y_inc;
            }
            error += dy2;
            x += x_inc;
        }
    }

    if(dy > dx){
        /*y loop movement*/
        for(i = 0; i < dy; i++){
            /*get pixel at current x and y*/
            p = (Uint8*)screen->pixels + y * screen->pitch + x * 4;
            /*set pixel to color*/
            *(Uint32*)p = color;

            if(error >= 0){
                error -= dy2;
                x += x_inc;
            }
            error += dx2;
            y += y_inc;
        }
    }

    SDL_UnlockSurface(screen);
}


/*draw contents of screen*/
void eng_flip()
{
    if(_eng->gl_enabled){
        SDL_GL_SwapWindow(_eng->window);
    }else{
        SDL_UpdateWindowSurface(_eng->window);
    }

}


/*color functions*/
color eng_createColor(int r, int g, int b, int a)
{
    color c;
    c.bgra[0] = b;
    c.bgra[1] = g;
    c.bgra[2] = r;
    c.bgra[3] = a;

    return c;
}

