#define SETUP
#define UPDATE
#define RENDER
#include "display.c"
#define RENDER_SHAPES_SDL_IMPLEMENTATION
#include "render_shapes_SDL.h"
#define VEC2_IMPLEMENTATION
#include "Vec2.h"

float lerp(float x0, float x1, float t);
Vec2 quad_bezier(Vec2 end1, Vec2 control, Vec2 end2, float t);
Vec2 cubic_bezier(Vec2 end1, Vec2 control1, Vec2 control2, Vec2 end2, float t);

void setup(void)
{
    limit_fps = 0;
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
    v3 = vec2_new(700, 500);

    if (left_button_pressed) {
        SDL_GetMouseState(&mouseX, &mouseY);
        v2 = vec2_new(mouseX, mouseY);
    } else {
        v2 = vec2_new(200, 500);
    }

    SDL_SetRenderDrawColor(renderer, Hex2ARGB(0xFF00FFFF));
    fill_circle(renderer, v0, 4);
    fill_circle(renderer, v1, 4);
    fill_circle(renderer, v2, 4);
    fill_circle(renderer, v3, 4);

    SDL_SetRenderDrawColor(renderer, Hex2ARGB(0xFFFFFFFF));
    for (float t = 0; t <= 1; t += 0.005) {
        // int x1 = lerp(v0.x, v2.x, t);
        // int x2 = lerp(v2.x, v1.x, t);
        // int y1 = lerp(v0.y, v2.y, t);
        // int y2 = lerp(v2.y, v1.y, t);
        // SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        Vec2 v = cubic_bezier(v0, v2, v3, v1, t);
        SDL_RenderDrawPoint(renderer, v.x, v.y);
    }
    // SDL_RenderDrawLine(renderer, v2.x, v2.y, v1.x, v1.y);
}

float lerp(float x0, float x1, float t)
{
    return x0 + (x1 - x0) * t;
}

Vec2 quad_bezier(Vec2 end1, Vec2 control, Vec2 end2, float t)
{
    Vec2 v;
    v.x = lerp(lerp(end1.x, control.x, t), lerp(control.x, end2.x, t), t);
    v.y = lerp(lerp(end1.y, control.y, t), lerp(control.y, end2.y, t), t);
    return v;
}

Vec2 cubic_bezier(Vec2 end1, Vec2 control1, Vec2 control2, Vec2 end2, float t)
{
    Vec2 temp1 = quad_bezier(end1, control1, control2, t);
    Vec2 temp2 = quad_bezier(control1, control2, end2, t);
    Vec2 v;
    v.x = (int)lerp(temp1.x, temp2.x, t);
    v.y = (int)lerp(temp1.y, temp2.y, t);

    return v;
}
