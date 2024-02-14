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

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define FPS 30
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

SDL_Color fps_color;

float scale = 2;
float damping = 0.9;
Mat buffer1, buffer2, temp;
int rows, cols;

int main()
{
    srand(time(0));
    game_is_running = !initialize_window();

    setup();

    while (game_is_running) {
        process_input();
        update();
        render();        
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

    rows = WINDOW_WIDTH/scale;
    cols = WINDOW_HEIGHT/scale;

    buffer1 = mat_alloc(rows, cols);
    buffer2 = mat_alloc(rows, cols);
    temp = mat_alloc(rows, cols);

    mat_fill(buffer1, 0);
    mat_rand(buffer1, 0, 1);

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
    static Uint32 previous_frame_time = 0;
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
    SDL_Rect rect;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            float value = MAT_AT(m, row, col);
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