#include "Vec2.h"
#define SETUP
#define UPDATE
#define RENDER
#include "display.c"
#define RENDER_SHAPES_SDL_IMPLEMENTATION
#include "render_shapes_SDL.h"
#define VEC2_IMPLEMENTATION
#include "Vec2.h"
#include "Almog_Dynamic_Array.h"

void setup(void)
{
    game_is_running = 0;
    Vec2 a = vec2_new(1, 2);
    Vec2 b = vec2_new(2, 3);
    Vec2 c;

    vec2_add(&c, &a, &b);
    VEC2_PRINT(c);
    
}

void update(void)
{

}

void render(void)
{

}
