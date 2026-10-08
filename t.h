/* t.h: turtle implementation in c99 with ngl.h */

/*
 * Copyright (C) 2026 Carlos G. S. 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact: Instagram DMs @cgs.dev
 *
 */


#ifndef _T_H
#define _T_H

#define NGL_UNSTRIP_PREFIX
#define NGL_DOUBLE_CHARS
#define NGL_NO_DOUBLE_BUFFERING
#define NGL_NO_FONTS
#include "thirdparty/ngl.h"

#ifndef T_STACK_SIZE
#define T_STACK_SIZE 1024
#endif /* T_STACK_SIZE */

#ifndef TDEF
#define TDEF static inline
#endif /* TDEF */

#define hex ngl_hex

typedef ngl_vec2_t  Vec2;
typedef ngl_color_t Color;
typedef u32         Key;

#define WHITE   NGL_WHITE
#define BLACK   NGL_BLACK
#define RED     NGL_RED
#define GREEN   NGL_GREEN
#define YELLOW  NGL_YELLOW
#define BLUE    NGL_BLUE
#define MAGENTA NGL_MAGENTA
#define CYAN    NGL_CYAN


/* the screen size is stored to these variables */
static u32 screen_w, screen_h;

/* initialize the turtle */
TDEF void t_init(void);

/* set target fps */
TDEF void t_target_fps(u32 fps);

/* update deltatime, enforce target fps, and present the screen to the terminal */
TDEF void t_update(void);

/* draw the turtle */
TDEF void t_draw(void);

/* clear the screen */
TDEF void t_clear(void);

/* free all the turtle's context */
TDEF void t_destroy(void);



/* make the program halt for s seconds */
TDEF void t_sleep(f32 s);

/* get deltatime */
TDEF f32  t_get_dt(void);

/* check if 'Q' or 'ESC' key is pressed */
TDEF bool t_should_close(void);


/* set the turtles color */
TDEF void t_color(Color color);


/* make the turtle leave a trace on the canvas */
TDEF void t_pendown(void);

/* make the turtle stop leaving a trace on the canvas */
TDEF void t_penup(void);


/* make the turtle step s steps forward */
TDEF void t_step(f32 steps);

/* make the turtle rotate by d degrees */
TDEF void t_rotate(f32 d);

/* set the turtle's position */
TDEF void t_pos(f32 x, f32 y);

/* set the turtle's angle to d degrees */
TDEF void t_angle(f32 d);

/* save the turtle's current position and angle */
TDEF void t_push(void);

/* load the previously saved position and angle */
TDEF void t_pop(void);


/* check if k is down */
TDEF bool t_is_key_down(Key k);

/* check if k is pressed */
TDEF bool t_is_key_pressed(Key k);

/* check if k is up */
TDEF bool t_is_key_up(Key k);

/* check if k is released */
TDEF bool t_is_key_released(Key k);


#define vec2_add       ngl_vec2_add
#define vec2_sub       ngl_vec2_sub
#define vec2_dot       ngl_vec2_dot
#define vec2_cross     ngl_vec2_cross
#define vec2_div       ngl_vec2_div
#define vec2_sqrt      ngl_vec2_sqrt
#define vec2_scale     ngl_vec2_scale
#define vec2_len       ngl_vec2_len
#define vec2_normalize ngl_vec2_normalize
#define vec2_rot90cw   ngl_vec2_rot90cw
#define vec2_rot90ccw  ngl_vec2_rot90ccw
#define vec2_mag       ngl_vec2_mag
#define vec2_mul       ngl_vec2_mul
#define vec2_rot       ngl_vec2_rot

#define vec2 ngl_vec2

typedef struct {
    size_t     top;
    ngl_vec2_t positions[T_STACK_SIZE];
    f32        angles[T_STACK_SIZE];
} TStack;

typedef struct {
    ngl_t  ngl;
    TStack stack;

    Vec2   pos, lastpos;
    f32    angle;

    u32    target_fps;
    f32    dt;

    Color  color;

    bool   pendown;
} T;

#endif /* _T_H */

#define T_IMPLEMENTATION
#ifdef T_IMPLEMENTATION

static T _t;

TDEF bool _tstack_push(Vec2 p, f32 a) {
    if (_t.stack.top >= T_STACK_SIZE) {
        return false;
    }

    _t.stack.positions[_t.stack.top] = p;
    _t.stack.angles[_t.stack.top]    = a;
    _t.stack.top++;

    return true;
}

TDEF bool _tstack_pop(Vec2 *p, f32 *a) {
    if (_t.stack.top == 0) {
        return false;
    }

    _t.stack.top--;

    *p = _t.stack.positions[_t.stack.top];
    *a = _t.stack.angles[_t.stack.top];

    return true;
}

TDEF void t_init(void) {
    _t.ngl   = ngl_new(0);

    _t.color = GREEN;

    _t.angle   = 0.0f;
    _t.pos     = vec2_scale(vec2(_t.ngl.screen.w, _t.ngl.screen.h), 0.5);
    _t.lastpos = _t.pos;

    _t.target_fps = 120;
    _t.dt         = ngl_get_dt(&_t.ngl);

    _t.pendown = false;

    screen_w = _t.ngl.screen.w;
    screen_h = _t.ngl.screen.h;

    ngl_fill_bg(&_t.ngl, ' ', _t.color);
    ngl_print_screen(&_t.ngl);
}

TDEF void t_destroy(void) {
    ngl_destroy(&_t.ngl);
    ngl_clear_screen();
}

TDEF void t_update(void) {
    ngl_get_keyboard_state(&_t.ngl);
    ngl_print_screen(&_t.ngl);

    _t.dt = ngl_get_dt(&_t.ngl);

    t_sleep((1.0f - _t.dt) / _t.target_fps);
}

TDEF void t_draw(void) {
    if (_t.pendown) {
        ngl_draw_line(&_t.ngl, _t.lastpos.x, _t.lastpos.y, _t.pos.x, _t.pos.y, 't', _t.color);
    }
    _t.lastpos = _t.pos;
}

TDEF void t_clear(void) {
    ngl_fill_bg(&_t.ngl, ' ', _t.color);
}

TDEF void t_sleep(f32 s) {
    ngl_delay(s * 1000.0f);
}

TDEF void t_target_fps(u32 fps) {
    _t.target_fps = fps;
}

TDEF f32 t_get_dt(void) {
    return _t.dt;
}

TDEF bool t_should_close(void) {
    return t_is_key_down(KEY_ESC) || t_is_key_down(KEY_Q);
}

TDEF void t_pos(f32 x, f32 y) {
    _t.pos = vec2(x, y);
    _t.lastpos = _t.pos;
}

TDEF void t_angle(f32 d) {
    _t.angle = d;
}

TDEF void t_step(f32 s) {
    Vec2 v = {0.0f, -s};

    _t.pos = vec2_add(_t.pos, vec2_rot(v, ngl_deg_to_rad(_t.angle)));
}

TDEF void t_rotate(f32 d) {
    _t.angle += d;
}

TDEF void t_color(Color c) {
    _t.color = c;
}

TDEF void t_pendown(void) {
    _t.pendown = true;
}

TDEF void t_penup(void) {
    _t.pendown = false;
}

TDEF void t_push(void) {
    _tstack_push(_t.pos, _t.angle);
}

TDEF void t_pop(void) {
    Vec2 p; f32 a;
    if (_tstack_pop(&p, &a)) {
        _t.pos = p;
        _t.angle = a;
        _t.lastpos = p;
    }
}

TDEF bool t_is_key_down(Key k) {
    return ngl_is_key_down(&_t.ngl, k);
}

TDEF bool t_is_key_pressed(Key k) {
    return ngl_is_key_pressed(&_t.ngl, k);
}

TDEF bool t_is_key_up(Key k) {
    return ngl_get_key_state(&_t.ngl, k) == KSTATE_UP;
}

TDEF bool t_is_key_released(Key k) {
    return ngl_is_key_released(&_t.ngl, k);
}

#define NGL_IMPLEMENTATION
#include "thirdparty/ngl.h"

#endif /* TU_IMPLEMENTATION */
