#ifndef CGM_INPUT_H
#define CGM_INPUT_H

#include "SDL.h"

/*keyboard, mouse, and joystick library*/

/*common key defines*/
#define KEY_UP SDL_SCANCODE_UP
#define KEY_RIGHT SDL_SCANCODE_RIGHT
#define KEY_DOWN SDL_SCANCODE_DOWN
#define KEY_LEFT SDL_SCANCODE_LEFT

#define JOY_DEADZONE 8000

typedef struct keyboardObj{
    Uint8* keys;
    Uint8* prev_keys;
    Uint8* key_state;
    int num_keys;
}keyboard;

typedef struct mouseObj{
    int x, y;
    int button; /*1 - left, 2 - middle, 3 - right*/
}mouse;

typedef struct joyObj{
    int id;
    int active;
    int axis; /* 0 = x_dir, 1 = y_dir*/
    int axis_value; /* -32768 to 32768*/
    double angle_ax;
	SDL_Joystick* controller;
}joy;


int input_init();
void input_close();
void input_update();

/*returns true (1) if button pressed
stores variables (x, y, button) in global mouse*/
int mouse_down();

/*getter mouse functions (called after mouse_down)*/
int mouse_getX();
int mouse_getY();
int mouse_getBtn();

int key_down(int key);
int key_up(int key);

extern keyboard* _key;
extern mouse* _ms;
extern joy* _joy;

#endif // CGM_INPUT_H
