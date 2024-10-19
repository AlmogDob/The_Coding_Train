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
#include <math.h>

typedef struct {
    size_t length;
    size_t capacity;
    Vec2* elements;
} ada_Vec2_array;

Vec2 a;
int num_of_points = 200;
float factor = 3;
float r;
ada_Vec2_array points;

void setup(void)
{
    a = vec2_new((int)window_width/2, (int)window_height/2);
    r = window_height/2;
    float delta = 2*PI / num_of_points;
    Vec2 currrent_point;

    ada_array(Vec2, points);
    for (int i = 0; i < num_of_points; i++) {
        float angle = i * delta + PI;
        currrent_point = vec2_new(window_width/2 + r * cos(angle), window_height/2 + r * sin(angle));
        ada_appand(Vec2, points, currrent_point);
    }
}

void update(void)
{
    a = vec2_new((int)window_width/2, (int)window_height/2);
    r = window_height/2;
    float delta = 2*PI / num_of_points;

    for (size_t i = 0; i < points.length; i++) {
        float angle = i * delta + PI;
        points.elements[i].x = window_width/2 + r * cos(angle);
        points.elements[i].y = window_height/2 + r * sin(angle);
    }
    
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_circle(renderer, a, r);

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (size_t i = 0; i < points.length; i++) {
        fill_circle(renderer, points.elements[i], 3);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (size_t i = 0; i < points.length; i++) {
        SDL_RenderDrawLine(renderer, points.elements[i].x, points.elements[i].y, points.elements[(int)(i*factor)%points.length].x, points.elements[(int)(i*factor)%points.length].y);
    }
}
