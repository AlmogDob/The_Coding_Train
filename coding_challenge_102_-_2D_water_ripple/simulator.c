/* This program is inspired by The Coding Train's video 'Coding Challenge 102: 2D Water Ripple'.
You can find the video here:
https://youtu.be/BZUdGqeOD0w .*/


#define STDIO
#include <stdio.h>

#define MATRIX_IMPLEMENTATION
#include "Matrix.h"

#define MATH
#include <math.h>

#define TIME
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 1600/1
#define WINDOW_HEIGHT 1000/1
#define FPS 100
#define FRAME_TARGET_TIME (1000 / FPS)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintF(expr) printf(#expr " = %g\n", expr)

#define PI 3.14159265359

int initialize_window(void);
void setup(void);
void process_input(void);
void update(void);
void render(void);
void destroy_window(void);
void fix_framerate(void);
void show_mat(SDL_Renderer * renderer, Mat m);
float liniar_map(float s, float min_in, float max_in, float min_out, float max_out);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Surface *text_surface = NULL;
SDL_Texture *text_texture = NULL;
SDL_Rect fps_place;
SDL_Color white_color;

int game_is_running = 0;
float delta_time;
float fps = 0;
int space_bar_was_pressed = 0;
int to_render = 1;
int to_update = 1;
Uint32 previous_frame_time = 0;

SDL_Color fps_color;

float scale = 5;
float damping = 0.99;
Mat buffer1, buffer2, temp;
int rows, cols, left_button_pressed = 0;
SDL_Rect rect;

int main()
{
    srand(time(0));
    game_is_running = !initialize_window();

    setup();

    while (game_is_running) {
        process_input();
        if (to_update) {
            update();
        }
        if (to_render) {
            render();
        }
        
    }

    destroy_window();

    return 0;
}

int initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return -1;
    }

    window = SDL_CreateWindow(NULL,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              0
                              );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Erorr creating SDL renderer.\n");
        return -1;
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Erorr initailizin SDL_ttf.\n");
        return -1;
    }

    font = TTF_OpenFont("./font/Gabriely Black.ttf",32);
    if (!font) {
        fprintf(stderr, "Error loading font.\n");
        return -1;
    }
    
    return 0;
}

void setup(void)
{
    white_color.a = 255;
    white_color.b = 255;
    white_color.g = 255;
    white_color.r = 255;

    fps_color = white_color;

    fps_place.x = 10;
    fps_place.y = 10;
    fps_place.w = 125;
    fps_place.h = 25;

    /*-----------------------------------*/

    // fps_color.a = 255;
    // fps_color.b = 113;
    // fps_color.g = 179;
    // fps_color.r = 60;

    rows = (int)(WINDOW_HEIGHT/scale);
    cols = (int)(WINDOW_WIDTH/scale);

    buffer1 = mat_alloc(rows, cols);
    buffer2 = mat_alloc(rows, cols);
    temp = mat_alloc(rows, cols);

    mat_fill(buffer1, 0);
    // mat_rand(buffer1, 0, 1);
    // MAT_AT(buffer1, rows/2, cols/2) = 1;

    for (int x = 0; x < cols; x++) {
        for (int y = 0; y < rows; y++) {
            MAT_AT(buffer2, y, x) = MAT_AT(buffer1, y, x);
        }
    }

}

void process_input(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game_is_running = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game_is_running = 0;
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    if (!space_bar_was_pressed) {
                        to_render = 0;
                        to_update = 0;
                        space_bar_was_pressed = 1;
                        break;
                    }
                    if (space_bar_was_pressed) {
                        to_render = 1;
                        to_update = 1;
                        previous_frame_time = SDL_GetTicks();
                        space_bar_was_pressed = 0;
                        break;
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_pressed = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_pressed = 0;
                }
                break;
        }
    }
}

void update(void)
{
    fix_framerate();

    fps = 1.0f / delta_time;

    char fps_count[100];
    sprintf(fps_count, "FPS = %8.4g", fps);
    text_surface = TTF_RenderText_Solid(font, fps_count,fps_color);

    text_texture = SDL_CreateTextureFromSurface(renderer,text_surface);
    SDL_FreeSurface(text_surface);

    /*----------------------------------------------------------------------------*/

    int mouse_x, mouse_y;

    if (left_button_pressed) {
        /*test*/
        // printf("left\n");
        /*test*/
        SDL_GetMouseState(&mouse_x, &mouse_y);
        MAT_AT(buffer1, (int)(mouse_y/scale), (int)(mouse_x/scale)) = 1;
      /*MAT_AT(buffer1, (int)(mouse_y/scale-1), (int)(mouse_x/scale)) = 0;
        MAT_AT(buffer1, (int)(mouse_y/scale+1), (int)(mouse_x/scale)) = 0;
        MAT_AT(buffer1, (int)(mouse_y/scale), (int)(mouse_x/scale+1)) = 0;
        MAT_AT(buffer1, (int)(mouse_y/scale), (int)(mouse_x/scale-1)) = 0;*/
    }

    for (int x = 1; x < cols -1; x++) {
        for (int y = 1; y < rows -1; y++) {
            MAT_AT(buffer2, y, x) = (MAT_AT(buffer1, y-1, x) + 
                                     MAT_AT(buffer1, y+1, x) + 
                                     MAT_AT(buffer1, y, x+1) + 
                                     MAT_AT(buffer1, y, x-1)) / 2 /*+ 
                                    (MAT_AT(buffer1, y-1, x-1) + 
                                     MAT_AT(buffer1, y+1, x+1) + 
                                     MAT_AT(buffer1, y-1, x+1) + 
                                     MAT_AT(buffer1, y+1, x-1)) / 2*/ - MAT_AT(buffer2, y, x); 

            MAT_AT(buffer2, y, x) = MAT_AT(buffer2, y, x) * damping;
        }
    }
    temp = buffer1;
    buffer1 = buffer2;
    buffer2 = temp;


}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    /*------------------------------------------------------------------------*/

    show_mat(renderer, buffer1);



    /*------------------------------------------------------------------------*/

    SDL_RenderCopy(renderer, text_texture, NULL, &fps_place);

    SDL_RenderPresent(renderer);
}

void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void fix_framerate(void)
{
    int time_ellapsed = SDL_GetTicks() - previous_frame_time;
    int time_to_wait = FRAME_TARGET_TIME - time_ellapsed;
    if (time_to_wait > 0 && time_to_wait < FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;
    previous_frame_time = SDL_GetTicks();
}

void show_mat(SDL_Renderer * renderer, Mat m)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            float value = MAT_AT(m, row, col);
            // value = liniar_map(value, 0, 3, 0.2, 1);
            SDL_SetRenderDrawColor(renderer, 255 * value, 255 * value, 255 * value, 255);
            rect.x = col * scale;
            rect.y = row * scale;
            rect.h = scale;
            rect.w = scale;
            // SDL_RenderDrawPoint(renderer, row * scale, col * scale);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

float liniar_map(float s, float min_in, float max_in, float min_out, float max_out)
{
    return (min_out + ((s-min_in)*(max_out-min_out))/(max_in-min_in));
}