#include "b_spline.h"
#include <stdlib.h>
#include <string.h>


// Constructor for BSpline
BSpline* b_spline_new(size_t count, SDL_Point* control_points) {
    BSpline* spline = (BSpline*)malloc(sizeof(BSpline));
    if (!spline) return NULL;

    spline->count = count;
    spline->knots = (float*)calloc(count + DEGREE + 1, sizeof(float));
    spline->control_points = (SDL_Point*)malloc(sizeof(SDL_Point) * count);
    spline->weights = (float*)calloc(count, sizeof(float));

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

void b_spline_calculate_knots(BSpline* spline) {
    size_t count = spline->count;
    float* knots = spline->knots;

    // Determine the number of interior knots
    size_t interiorKnotsCount = count - DEGREE + 1;

    // Calculate the knot values for the interior knots
    float step = 1.0f / (interiorKnotsCount + 1);
    for (size_t i = 0; i < interiorKnotsCount; ++i) {
        knots[i + DEGREE] = (i + 1) * step;
    }

    // Set the boundary knots
    for (size_t i = 0; i < DEGREE; ++i) {
        knots[i] = 0.0f;
        knots[interiorKnotsCount + DEGREE + i] = 1.0f;
    }
}

// Calculates the control point weights for the spline
void b_spline_calculate_coefficients(BSpline* spline) {
    for (size_t i = 0; i < spline->count; ++i) {
        spline->weights[i] = 1.0f;
    }
}

// Helper function to calculate the basis function
float b_spline_basis(int i, int degree, float t, float* knots) {
    if (degree == 0) {
        return (knots[i] <= t && t < knots[i+1]) ? 1.0f : 0.0f;
    } else {
        float a = (knots[i+degree] != knots[i]) ? (t - knots[i]) / (knots[i+degree] - knots[i]) : 0.0f;
        float b = (knots[i+degree+1] != knots[i+1]) ? (knots[i+degree+1] - t) / (knots[i+degree+1] - knots[i+1]) : 0.0f;
        return a * b_spline_basis(i, degree-1, t, knots) + b * b_spline_basis(i+1, degree-1, t, knots);
    }
}

float b_spline_evaluate(BSpline* spline, float t) {
    size_t count = spline->count;
    float* knots = spline->knots;
    SDL_Point* control_points = spline->control_points;

    float sum = 0.0f;
    for (size_t i = 0; i < count; ++i) {
        float basis = b_spline_basis(i, DEGREE, t, knots);
        sum += control_points[i].y * basis;
    }

    return sum;
}

