#include "drawing_lib.h"

/*============FUNCTION_DECLARATION============*/

void VectorCalculateMandelbrotSet(Picture* picture);

/*============================================*/

int main()
{
    Picture* mandelbrot_picture = PictureCtor(X_SHIFT, Y_SHIFT, ZOOM_DEFAULT);

    DrawWindow(mandelbrot_picture, VectorCalculateMandelbrotSet);

    PictureDtor(mandelbrot_picture);

    return 0;
}

void VectorCalculateMandelbrotSet(Picture* picture)
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix += 8)
        {
            float x0 = picture->x_shift + (((float)ix - SCREEN_WIDTH  / 2) * dX) * picture->zoom;
            float y0 = picture->y_shift + (((float)iy - SCREEN_HEIGHT / 2) * dY) * picture->zoom * HEIGHT_WIDTH_RELATION;

            float zoom_dx = dX * picture->zoom;

            float x0_array[8]    = {x0, x0 + zoom_dx, x0 + 2*zoom_dx, x0 + 3*zoom_dx, x0 + 4*zoom_dx, x0 + 5*zoom_dx, x0 + 6*zoom_dx, x0 + 7*zoom_dx};
            float y0_array[8]    = {y0, y0,           y0,             y0,             y0,             y0,             y0,             y0            };

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