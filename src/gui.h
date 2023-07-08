#ifndef GUI_H
#define GUI_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "b_spline.h"


typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Point* control_points;
    size_t control_point_count;
    size_t control_point_capacity;
    BSpline* spline;
    bool quit;
    TTF_Font* font;
    SDL_Rect reset_button;
    SDL_Rect exit_button;
} Gui;

Gui* gui_new();
void gui_run(Gui* gui);
void gui_free(Gui* gui);

#endif // GUI_H
