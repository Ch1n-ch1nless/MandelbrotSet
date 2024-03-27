#include "drawing_lib.h"

/*============FUNCTION_DECLARATION============*/

void CalculateMandelbrotSet_With_out_SSE(Picture* picture);

void CalculateMandelbrotSet_With_Fake_SSE(Picture* picture);

/*============================================*/

int main()
{
    Picture* mandelbrot_picture = PictureCtor(X_SHIFT, Y_SHIFT, ZOOM_DEFAULT);

    DrawWindow(mandelbrot_picture, CalculateMandelbrotSet_With_Fake_SSE);

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

void CalculateMandelbrotSet_With_Fake_SSE(Picture* picture)
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix += 8)
        {
            float x0             = (((float)ix - SCREEN_WIDTH  / 2) * dX + picture->x_shift) * picture->zoom;
            float y0             = (((float)iy - SCREEN_HEIGHT / 2) * dY + picture->y_shift) * picture->zoom;

            float zoom_dx = dX * picture->zoom;

            float x0_array[8]    = {x0, x0 + zoom_dx, x0 + 2*zoom_dx, x0 + 3*zoom_dx, x0 + 4*zoom_dx, x0 + 5*zoom_dx, x0 + 6*zoom_dx, x0 + 7*zoom_dx};
            float y0_array[8]    = {y0, y0,           y0,             y0,             y0,             y0,             y0,             y0       };

            float cur_x_array[8] = {}; for (int i = 0; i < 8; i++) cur_x_array[i] = x0_array[i];
            float cur_y_array[8] = {}; for (int i = 0; i < 8; i++) cur_y_array[i] = y0_array[i];

            int iter_array[8]    = {0, 0, 0, 0, 0, 0, 0, 0};

            for (int i = 0; i < MAX_NUMBER_OF_ITERATIONS; i++)
            {
                float x2_array[8] = {}; for (int i = 0; i < 8; i++) x2_array[i] = cur_x_array[i] * cur_x_array[i];
                float y2_array[8] = {}; for (int i = 0; i < 8; i++) y2_array[i] = cur_y_array[i] * cur_y_array[i];
                float xy_array[8] = {}; for (int i = 0; i < 8; i++) xy_array[i] = cur_x_array[i] * cur_y_array[i];
                
                float r2_array[8] = {}; for (int i = 0; i < 8; i++) r2_array[i] = x2_array[i] + y2_array[i];

                int is_point[8]   = {0, 0, 0, 0, 0, 0, 0, 0};

                for (int i = 0; i < 8; i++) is_point[i] = (r2_array[i] < MAX_SQUARE_RADIUS);

                int bit_mask = 0;
                for (int i = 0; i < 8; i++) bit_mask |= (is_point[i] << i);

                if (!bit_mask) break;

                for (int i = 0; i < 8; i++) iter_array[i] += is_point[i];

                for (int i = 0; i < 8; i++) cur_x_array[i] = x2_array[i] - y2_array[i] + x0_array[i];
                for (int i = 0; i < 8; i++) cur_y_array[i] = xy_array[i] + xy_array[i] + y0_array[i];
            }

            for (int i = 0; i < 8; i++)
            {
                SetPixel(picture->pixel_array, ix + i, iy, iter_array[i]);
            }
        }
    }
}