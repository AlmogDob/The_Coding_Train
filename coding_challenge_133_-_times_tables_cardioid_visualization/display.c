#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH 800
#endif

#ifndef WINDOW_HEIGHT
#define WINDOW_HEIGHT 600
#endif

#ifndef FPS
#define FPS 30
#endif

#define FRAME_TARGET_TIME (1000 / FPS)
#define dprintINT(expr) printf(#expr " = %d\n", expr)
#define dprintF(expr) printf(#expr " = %g\n", expr)

#ifndef PI
#define PI M_PI
#endif

int initialize_window(void);
void setup_window(void);
void process_input_window(void);
void update_window(void);
void render_window(void);
void destroy_window(void);
void fix_framerate(void);
void setup(void);
void update(void);
void render(void);
float liniar_map(float s, float min_in, float max_in, float min_out, float max_out);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
SDL_Surface *text_surface = NULL;
SDL_Texture *text_texture = NULL;
SDL_Rect fps_place;
SDL_Color white_color;
SDL_Color fps_color;

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
size_t previous_frame_time = 0;
int left_button_pressed = 0;
int to_limit_fps = 1;
int window_width, window_height;

int main()
{
    game_is_running = !initialize_window();

    setup_window();

    while (game_is_running) {
        process_input_window();
        if (to_update) {
            update_window();
        }
        if (to_render) {
            render_window();
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
                              SDL_WINDOW_RESIZABLE
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

void setup_window(void)
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

    window_width = WINDOW_WIDTH;
    window_height = WINDOW_HEIGHT;

    /*-----------------------------------*/

    setup();

}

void process_input_window(void)
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
                if (event.key.keysym.sym == SDLK_q) {
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

void update_window(void)
{
    fix_framerate();

    fps = 1.0f / delta_time;

    char fps_count[100];
    sprintf(fps_count, "FPS = %8.4g", fps);
    text_surface = TTF_RenderText_Solid(font, fps_count,fps_color);

    text_texture = SDL_CreateTextureFromSurface(renderer,text_surface);
    SDL_FreeSurface(text_surface);

    SDL_GetWindowSize(window, &window_width, &window_height);

    /*----------------------------------------------------------------------------*/

    update();

}

void render_window(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    /*------------------------------------------------------------------------*/

    render();

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
        if (to_limit_fps) {
            SDL_Delay(time_to_wait);
        }
    }
    delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;
    previous_frame_time = SDL_GetTicks();
}

#ifndef SETUP
#define SETUP
void setup(void) {}
#endif

#ifndef UPDATE
#define UPDATE
void update(void) {}
#endif

#ifndef RENDER
#define RENDER
void render(void) {}
#endif



void show_mat(SDL_Renderer * renderer, Mat m)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            float value = MAT_AT(m, row, col);
            value = liniar_map(value, 0, 3, 0.2, 1);
            SDL_SetRenderDrawColor(renderer, 100 * value, 200 * value, 255 * value, 255);
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