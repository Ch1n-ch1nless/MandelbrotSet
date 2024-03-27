#include "drawing_lib.h"

/*============FUNCTION_DECLARATION============*/

void CalculateMandelbrotSet_With_out_SSE(Picture* picture);

/*============================================*/

int main()
{
    Picture* mandelbrot_picture = PictureCtor(X_SHIFT, Y_SHIFT, ZOOM);

    DrawWindow(mandelbrot_picture, CalculateMandelbrotSet_With_out_SSE);

    PictureDtor(mandelbrot_picture);

    return 0;
}

/*===========================================================*/

void CalculateMandelbrotSet_With_out_SSE(Picture* picture)
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix++)
        {
            float x0 = (((float)ix - SCREEN_WIDTH  / 2) * dX + picture->x_shift) * picture->zoom;
            float y0 = (((float)iy - SCREEN_HEIGHT / 2) * dY + picture->y_shift) * picture->zoom;

            float x = x0;
            float y = y0;

            int number_of_iterations = 0;

            for ( ; number_of_iterations < MAX_NUMBER_OF_ITERATIONS; number_of_iterations++)
            {
                float x2 = x * x;
                float y2 = y * y;
                float xy = x * y;

                float r2 = x2 + y2;
                if (r2 > MAX_SQUARE_RADIUS) break;

                x = x2 - y2 + x0;
                y = xy + xy + y0;
            }

            SetPixel(picture->pixel_array, ix, iy, number_of_iterations);
        }
    }
}