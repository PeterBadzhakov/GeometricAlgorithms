#include "b_spline.h"
#include <stdlib.h>
#include <string.h>

// Constructor for BSpline
BSpline* b_spline_new(size_t count, SDL_Point* control_points) {
    BSpline* spline = (BSpline*)malloc(sizeof(BSpline));
    if (!spline) return NULL;

    spline->count = count;
    spline->knots = (float*)malloc(sizeof(float) * (count + DEGREE + 1));
    spline->control_points = (SDL_Point*)malloc(sizeof(SDL_Point) * count);
    spline->weights = (float*)malloc(sizeof(float) * count);

    if (!spline->knots || !spline->control_points || !spline->weights) {
        b_spline_free(spline);
        return NULL;
    }

    memcpy(spline->control_points, control_points, sizeof(SDL_Point) * count);
    b_spline_calculate_knots(spline);
    b_spline_calculate_coefficients(spline);
    return spline;
}

// Destructor for BSpline
void b_spline_free(BSpline* spline) {
    if (spline) {
        free(spline->knots);
        free(spline->control_points);
        free(spline->weights);
        free(spline);
    }
}

// Calculates the knots vector for the spline
void b_spline_calculate_knots(BSpline* spline) {
    size_t count = spline->count;
    float* knots = spline->knots;

    for (size_t i = 0; i < count + DEGREE + 1; ++i) {
        if (i <= DEGREE) {
            knots[i] = 0.0f;
        }
        else if (i < count) {
            knots[i] = (float)(i - DEGREE);
        }
        else {
            knots[i] = (float)(count - DEGREE);
        }
    }
}

// Calculates the control point weights for the spline
void b_spline_calculate_coefficients(BSpline* spline) {
    for (size_t i = 0; i < spline->count; ++i) {
        spline->weights[i] = 1.0f;
    }
}

// Evaluates the spline at the parameter t
float b_spline_evaluate(BSpline* spline, float t) {
    size_t count = spline->count;
    float* knots = spline->knots;
    SDL_Point* control_points = spline->control_points;
    float* weights = spline->weights;

    for (size_t i = 0; i < count; ++i) {
        if (t >= knots[i] && t <= knots[i + 1]) {
            weights[i] *= (t - knots[i]) / (knots[i + 1] - knots[i]);
        }
        else {
            weights[i] = 0.0f;
        }
    }

    float sum = 0.0f;
    for (size_t i = 0; i < count; ++i) {
        sum += control_points[i].y * weights[i];
    }

    return sum;
}
