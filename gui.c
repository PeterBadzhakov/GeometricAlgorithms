#include "gui.h"
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BUTTON_HEIGHT 30
#define BUTTON_WIDTH 100
#define CONTROL_POINT_RADIUS 3
#define MAX_CONTROL_POINTS 100

Gui* gui_new() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return NULL;
    }

    if (TTF_Init() == -1) {
        return NULL;
    }

    Gui* gui = (Gui*)malloc(sizeof(Gui));
    if (!gui) return NULL;

    gui->window = SDL_CreateWindow("B-Spline Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gui->window) {
        gui_free(gui);
        return NULL;
    }

    gui->renderer = SDL_CreateRenderer(gui->window, -1, SDL_RENDERER_ACCELERATED);
    if (!gui->renderer) {
        gui_free(gui);
        return NULL;
    }

    gui->control_points = (SDL_Point*)malloc(sizeof(SDL_Point) * MAX_CONTROL_POINTS);
    if (!gui->control_points) {
        gui_free(gui);
        return NULL;
    }

    gui->control_point_count = 0;
    gui->control_point_capacity = MAX_CONTROL_POINTS;
    gui->spline = NULL;
    gui->quit = false;

    gui->font = TTF_OpenFont("C:/Windows/Fonts/Times.ttf", 16); // Make sure to change the font path
    if (!gui->font) {
        gui_free(gui);
        return NULL;
    }

    gui->reset_button.x = WINDOW_WIDTH - BUTTON_WIDTH;
    gui->reset_button.y = 0;
    gui->reset_button.w = BUTTON_WIDTH;
    gui->reset_button.h = BUTTON_HEIGHT;

    gui->exit_button.x = WINDOW_WIDTH - BUTTON_WIDTH;
    gui->exit_button.y = BUTTON_HEIGHT;
    gui->exit_button.w = BUTTON_WIDTH;
    gui->exit_button.h = BUTTON_HEIGHT;

    return gui;
}

void gui_free(Gui* gui) {
    if (gui) {
        if (gui->window) SDL_DestroyWindow(gui->window);
        if (gui->renderer) SDL_DestroyRenderer(gui->renderer);
        free(gui->control_points);
        b_spline_free(gui->spline);
        TTF_CloseFont(gui->font);
        free(gui);
        SDL_Quit();
    }
}

void gui_run(Gui* gui) {
    while (!gui->quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                gui->quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (SDL_PointInRect(&((SDL_Point) { x, y }), &(gui->reset_button))) {
                    gui->control_point_count = 0;
                }
                else if (SDL_PointInRect(&((SDL_Point) { x, y }), &(gui->exit_button))) {
                    gui->quit = true;
                }
                else if (gui->control_point_count < gui->control_point_capacity) {
                    gui->control_points[gui->control_point_count++] = (SDL_Point){ x, y };
                    b_spline_free(gui->spline);
                    gui->spline = b_spline_new(gui->control_point_count, gui->control_points);
                }
            }
        }

        SDL_SetRenderDrawColor(gui->renderer, 255, 255, 255, 255);
        SDL_RenderClear(gui->renderer);

        SDL_SetRenderDrawColor(gui->renderer, 0, 0, 0, 255);
        if (gui->spline) {
            float step = 1.0f / WINDOW_WIDTH;
            for (float t = 0.0f; t <= 1.0f; t += step) {
                int x = (int)(t * WINDOW_WIDTH);
                int y = (int)b_spline_evaluate(gui->spline, t);
                SDL_RenderDrawPoint(gui->renderer, x, y);
            }
        }

        SDL_SetRenderDrawColor(gui->renderer, 255, 0, 0, 255);
        for (size_t i = 0; i < gui->control_point_count; ++i) {
            SDL_Rect rect = { gui->control_points[i].x - CONTROL_POINT_RADIUS, gui->control_points[i].y - CONTROL_POINT_RADIUS, CONTROL_POINT_RADIUS * 2, CONTROL_POINT_RADIUS * 2 };
            SDL_RenderFillRect(gui->renderer, &rect);
        }

        SDL_SetRenderDrawColor(gui->renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(gui->renderer, &(gui->reset_button));
        SDL_RenderFillRect(gui->renderer, &(gui->exit_button));

        SDL_Color black = { 0, 0, 0 };
        SDL_Surface* reset_surface = TTF_RenderText_Solid(gui->font, "Reset", black);
        SDL_Texture* reset_texture = SDL_CreateTextureFromSurface(gui->renderer, reset_surface);
        SDL_RenderCopy(gui->renderer, reset_texture, NULL, &(gui->reset_button));

        SDL_Surface* exit_surface = TTF_RenderText_Solid(gui->font, "Exit", black);
        SDL_Texture* exit_texture = SDL_CreateTextureFromSurface(gui->renderer, exit_surface);
        SDL_RenderCopy(gui->renderer, exit_texture, NULL, &(gui->exit_button));

        SDL_RenderPresent(gui->renderer);

        SDL_FreeSurface(reset_surface);
        SDL_DestroyTexture(reset_texture);
        SDL_FreeSurface(exit_surface);
        SDL_DestroyTexture(exit_texture);
    }
}
