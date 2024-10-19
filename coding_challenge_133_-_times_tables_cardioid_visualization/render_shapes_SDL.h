#ifndef RENDER_SHAPES_SDL_H_
#define RENDER_SHAPES_SDL_H_

#include <math.h>
#include <SDL2/SDL.h>
#include "Vec2.h"

void draw_circle(SDL_Renderer *renderer, Vec2 center, float r);

#ifdef RENDER_SHAPES_SDL_IMPLEMENTATION

void draw_circle(SDL_Renderer *renderer, Vec2 center, float r)
{
    for (int dy = -r; dy <= r; dy++) {
        for (int dx = -r; dx <= r; dx ++) {
            if (dx * dx + dy * dy == r * r) {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }

}

#endif /*RENDER_SHAPES_SDL_IMPLEMENTATION*/

#endif /*RENDER_SHAPES_SDL_H_*/
