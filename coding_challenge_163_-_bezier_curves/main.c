#define SETUP
#define UPDATE
#define RENDER
#include "display.c"
#define RENDER_SHAPES_SDL_IMPLEMENTATION
#include "render_shapes_SDL.h"
#define VEC2_IMPLEMENTATION
#include "Vec2.h"

float lerp(float x0, float x1, float t);
float quad_bezier(float end1, float control, float end2, float t);

void setup(void)
{

}

void update(void)
{
    
}

void render(void)
{
    Vec2 v0, v1, v2, v3;
    int mouseX, mouseY;

    v0 = vec2_new(50, 300);
    v1 = vec2_new(700, 200);
    v2 = vec2_new(200, 500);

    if (left_button_pressed) {
        SDL_GetMouseState(&mouseX, &mouseY);
        v3 = vec2_new(mouseX, mouseY);
    } else {
        v3 = vec2_new(700, 500);
    }

    SDL_SetRenderDrawColor(renderer, Hex2ARGB(0xFF00FFFF));
    fill_circle(renderer, v0, 4);
    fill_circle(renderer, v1, 4);
    fill_circle(renderer, v2, 4);
    fill_circle(renderer, v3, 4);

    SDL_SetRenderDrawColor(renderer, Hex2ARGB(0xFFFFFFFF));
    for (float t = 0; t <= 1; t += 0.025) {
        // int x1 = lerp(v0.x, v2.x, t);
        // int x2 = lerp(v2.x, v1.x, t);
        // int y1 = lerp(v0.y, v2.y, t);
        // int y2 = lerp(v2.y, v1.y, t);
        // SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        int x1 = quad_bezier(v0.x, v2.x, v3.x, t);
        int y1 = quad_bezier(v0.y, v2.y, v3.y, t);
        int x2 = quad_bezier(v2.x, v3.x, v1.x, t);
        int y2 = quad_bezier(v2.y, v3.y, v1.y, t);
        int x = (int)lerp(x1, x2, t);
        int y = (int)lerp(y1, y2, t);
        SDL_RenderDrawPoint(renderer, x, y);
    }
    // SDL_RenderDrawLine(renderer, v2.x, v2.y, v1.x, v1.y);
}

float lerp(float x0, float x1, float t)
{
    return x0 + (x1 - x0) * t;
}

float quad_bezier(float end1, float control, float end2, float t)
{
        return lerp(lerp(end1, control, t), lerp(control, end2, t), t);
}
