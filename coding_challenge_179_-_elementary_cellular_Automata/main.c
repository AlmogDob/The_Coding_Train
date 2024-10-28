#define SETUP
#define UPDATE
#define RENDER
#include "display.c"
#define MATRIX_IMPLEMENTATION
#include "Matrix.h"

#define RULE_LEN 8

void render_grid(SDL_Renderer *renderer, Mat grid);
void update_grid(Mat next_grid, Mat current_grid);
int calculate_state(int left, int right, int state);
void number2binary(int number, char* binary, int min_length);

int cell_size = 6, rule_num = 110;
int y = -1;
Mat current_grid;
Mat next_grid;
char rule[RULE_LEN+1];

void setup(void)
{
    to_clear_window = 0;
    limit_fps = 0;

    current_grid = mat_alloc(1, WINDOW_WIDTH/cell_size);
    next_grid = mat_alloc(1, WINDOW_WIDTH/cell_size);

    mat_fill(current_grid, 0);
    mat_fill(next_grid, 0);

    dprintSIZE_T(current_grid.cols);

    MAT_AT(current_grid, 0, (current_grid.cols+1)/2) = 1;
    
    number2binary(rule_num, rule, RULE_LEN);
    printf("%s\n", rule);
}

void update(void)
{
    y++;
    if (y) {
        update_grid(next_grid, current_grid);

        mat_copy(current_grid, next_grid);
    }

    
}

void render(void)
{
    render_grid(renderer, current_grid);

    if (y*cell_size >= WINDOW_HEIGHT) {
        to_clear_window = 1;
        y = -1;
    } else {
        to_clear_window = 0;
    }

}

void render_grid(SDL_Renderer *renderer, Mat grid)
{
    SDL_Rect temp_rect = {.h = cell_size, .w = cell_size};

    for (size_t j = 0; j < grid.cols; j++) {
        temp_rect.x = (WINDOW_WIDTH - cell_size * grid.cols)/2 + j*cell_size;
        temp_rect.y = y*cell_size + cell_size;
        SDL_SetRenderDrawColor(renderer, 255 - 255*MAT_AT(grid, 0, j),
                                            255 - 255*MAT_AT(grid, 0, j),
                                            255 - 255*MAT_AT(grid, 0, j),
                                            255);
        SDL_RenderFillRect(renderer, &temp_rect);

        SDL_SetRenderDrawColor(renderer, Hex2ARGB(0x00000000));
        SDL_RenderDrawRect(renderer, &temp_rect);
    }
}

void update_grid(Mat next_grid, Mat current_grid)
{
    for (size_t j = 0; j < current_grid.cols; j++) {
        int left  = MAT_AT(current_grid, 0, (j-1 + current_grid.cols)% current_grid.cols);
        int right = MAT_AT(current_grid, 0, (j+1 + current_grid.cols)% current_grid.cols);
        int state = MAT_AT(current_grid, 0, (j + current_grid.cols)% current_grid.cols);
        MAT_AT(next_grid, 0, j) = calculate_state(left, right, state);
    }

}

int calculate_state(int left, int right, int state)
{
    int index = 4*left + 2*state + 1*right;

    char *temp_p, temp;
    
    temp = rule[RULE_LEN - 1 -index];
    temp_p = &temp;

    return atoi(temp_p);
}

void number2binary(int number, char* binary, int min_length)
{
    int counter = 0; 
    char temp;

    while (number) {
        if (number%2) {
            binary[counter++] = '1';
        } else {
            binary[counter++] = '0';
        }

        number = number/2;
    }

    while (counter < min_length) {
        binary[counter] = '0';
        counter++;
    }

    for (int i = 0; i < counter/2; i++) {
        temp = binary[i];
        binary[i] = binary[counter-i-1];
        binary[counter-i-1] = temp;
    }

    binary[counter] = '\0';
    
}
