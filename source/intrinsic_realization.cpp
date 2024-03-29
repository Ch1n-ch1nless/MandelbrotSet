#include "drawing_lib.h"

/*============FUNCTION_DECLARATION============*/

void IntrinsicCalculateMandelbrotSet(Picture* picture);

/*============================================*/

int main()
{
    Picture* mandelbrot_picture = PictureCtor(X_SHIFT, Y_SHIFT, ZOOM_DEFAULT);

    DrawWindow(mandelbrot_picture, IntrinsicCalculateMandelbrotSet);

    PictureDtor(mandelbrot_picture);

    return 0;
}

void IntrinsicCalculateMandelbrotSet(Picture* picture)
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    __m256 R2Max        = _mm256_set1_ps(MAX_SQUARE_RADIUS);
    __m256 mask_array   = _mm256_set1_ps(1.f);
    __m256 _76543210    = _mm256_set_ps(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f);
    __m256 delta_x      = _mm256_set1_ps(dX * picture->zoom);

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix += 8)
        {
            float x0 = picture->x_shift + (((float)ix - SCREEN_WIDTH  / 2) * dX) * picture->zoom;
            float y0 = picture->y_shift + (((float)iy - SCREEN_HEIGHT / 2) * dY) * picture->zoom * HEIGHT_WIDTH_RELATION;

            __m256 x0_coords = _mm256_add_ps(_mm256_mul_ps(delta_x, _76543210), _mm256_set1_ps(x0));
            __m256 y0_coords = _mm256_set1_ps(y0);

            __m256 x_coords = x0_coords;
            __m256 y_coords = y0_coords;

            union 
            {
                __m256i iterations = _mm256_set1_epi32(0);
                int iter_array[8];
            };

            for (int i = 0; i < MAX_NUMBER_OF_ITERATIONS; i++)
            {
                __m256 x2 = _mm256_mul_ps(x_coords, x_coords);
                __m256 y2 = _mm256_mul_ps(y_coords, y_coords);
                __m256 xy = _mm256_mul_ps(x_coords, y_coords);

                __m256 r2 = _mm256_add_ps(x2, y2);

                __m256 cmp = _mm256_cmp_ps(r2, R2Max, _CMP_LT_OS);
                int bitmask = _mm256_movemask_ps(cmp);

                if (!bitmask)
                {
                    break;
                }

                iterations = _mm256_add_epi32(_mm256_cvttps_epi32(_mm256_and_ps(cmp, mask_array)), iterations);

                x_coords = _mm256_add_ps(_mm256_sub_ps(x2, y2), x0_coords);
                y_coords = _mm256_add_ps(_mm256_add_ps(xy, xy), y0_coords);
            }

            for (int i = 0; i < 8; i++)
            {
                SetPixel(picture->pixel_array, ix + i, iy, iter_array[i]);
            }
        }
    }
}
