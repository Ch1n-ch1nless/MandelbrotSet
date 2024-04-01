#ifndef CONFIG_H
#define CONFIG_H

#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdio.h>

/*==================CONSTANTS=================*/

const int       MAX_NUMBER_OF_ITERATIONS    = 256;

const int       SCREEN_WIDTH                = 800;
const int       SCREEN_HEIGHT               = 600;

const float     MAX_SQUARE_RADIUS           =  100.0f;
const float     dX                          =  1.0f/400.f;
const float     dY                          =  1.0f/300.f;
const float     X_SHIFT                     = -0.75f;
const float     Y_SHIFT                     =  0.0f;
const float     ZOOM_DEFAULT                =  1.0f;
const float     HEIGHT_WIDTH_RELATION       =  ((float)SCREEN_HEIGHT) / ((float)SCREEN_WIDTH);

const float     SHIFT_CONSTANT              = 0.1f;
const float     ZOOM_CONSTANT               = 1.1f;

/*===================STRUCTS==================*/

struct Coords
{
    float   x_shift = X_SHIFT;
    float   y_shift = Y_SHIFT;
    float   zoom    = ZOOM_DEFAULT;
};

/*============FUNCTION_DECLARATION============*/

void PerPixelCalculateMandelbrotSet(unsigned int* pixel_array, Coords* coords_begin);
void VectorCalculateMandelbrotSet(unsigned int* pixel_array, Coords* coords_begin);
void IntrinsicCalculateMandelbrotSet(unsigned int* pixel_array, Coords* coords_begin);

/*============================================*/

#endif