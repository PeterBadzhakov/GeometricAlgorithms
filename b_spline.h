#ifndef B_SPLINE_H
#define B_SPLINE_H

#include <stddef.h>
#include <SDL.h>

#define DEGREE 2

typedef struct {
    size_t count;
    float* knots;
    SDL_Point* control_points;
    float* weights;
} BSpline;

BSpline* b_spline_new(size_t count, SDL_Point* control_points);
void b_spline_free(BSpline* spline);
void b_spline_calculate_knots(BSpline* spline);
void b_spline_calculate_coefficients(BSpline* spline);
float b_spline_evaluate(BSpline* spline, float t);

#endif // B_SPLINE_H
