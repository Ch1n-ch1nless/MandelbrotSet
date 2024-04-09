#include "config.h"

void PerPixelCalculateMandelbrotSet(unsigned int* pixel_array, Coords* coords_begin)
{
    assert((coords_begin != nullptr) && "Pointer to \'coords_begin\' is NULL!!!\n");
    assert((pixel_array  != nullptr) && "Pointer to \'pixel_array\'  is NULL!!!\n");

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix++)
        {
            float x0 = coords_begin->x_shift + (((float)ix - SCREEN_WIDTH  / 2) * dX) * coords_begin->zoom;
            float y0 = coords_begin->y_shift + (((float)iy - SCREEN_HEIGHT / 2) * dY) * coords_begin->zoom * HEIGHT_WIDTH_RELATION;

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

            pixel_array[iy * SCREEN_WIDTH + ix] = number_of_iterations; 
        }
    }
}

void VectorCalculateMandelbrotSet(unsigned int* pixel_array, Coords* coords_begin)
{
    assert((coords_begin != nullptr) && "Pointer to \'coords_begin\' is NULL!!!\n");
    assert((pixel_array  != nullptr) && "Pointer to \'pixel_array\'  is NULL!!!\n");

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix += 8)
        {
            float x0 = coords_begin->x_shift + (((float)ix - SCREEN_WIDTH  / 2) * dX) * coords_begin->zoom;
            float y0 = coords_begin->y_shift + (((float)iy - SCREEN_HEIGHT / 2) * dY) * coords_begin->zoom * HEIGHT_WIDTH_RELATION;

            float zoom_dx = dX * coords_begin->zoom;

            float x0_array[8]    = {x0, x0 + zoom_dx, x0 + 2*zoom_dx, x0 + 3*zoom_dx, x0 + 4*zoom_dx, x0 + 5*zoom_dx, x0 + 6*zoom_dx, x0 + 7*zoom_dx};
            float y0_array[8]    = {y0, y0,           y0,             y0,             y0,             y0,             y0,             y0            };

            float cur_x_array[8] = {}; for (int i = 0; i < 8; i++) cur_x_array[i] = x0_array[i];
            float cur_y_array[8] = {}; for (int i = 0; i < 8; i++) cur_y_array[i] = y0_array[i];

            int iter_array[8]    = {};

            for (int iter = 0; iter < MAX_NUMBER_OF_ITERATIONS; iter++)
            {
                float x2_array[8] = {}; for (int i = 0; i < 8; i++) x2_array[i] = cur_x_array[i] * cur_x_array[i];
                float y2_array[8] = {}; for (int i = 0; i < 8; i++) y2_array[i] = cur_y_array[i] * cur_y_array[i];
                float xy_array[8] = {}; for (int i = 0; i < 8; i++) xy_array[i] = cur_x_array[i] * cur_y_array[i];
                
                float r2_array[8] = {}; for (int i = 0; i < 8; i++) r2_array[i] = x2_array[i] + y2_array[i];

                int is_point[8]   = {};

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
                pixel_array[iy * SCREEN_WIDTH + ix + i] = iter_array[i];
            }
        }
    }
}

void AVXCalculateMandelbrotSet(unsigned int* pixel_array, Coords* coords_begin)
{
    assert((coords_begin != nullptr) && "Pointer to \'coords_begin\' is NULL!!!\n");
    assert((pixel_array  != nullptr) && "Pointer to \'pixel_array\'  is NULL!!!\n");

    __m256 R2Max        = _mm256_set1_ps(MAX_SQUARE_RADIUS);
    __m256 mask_array   = _mm256_set1_ps(1.f);
    __m256 _76543210    = _mm256_set_ps(7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.0f);
    __m256 delta_x      = _mm256_set1_ps(dX * coords_begin->zoom);

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix += 8)
        {
            float x0 = coords_begin->x_shift + (((float)ix - SCREEN_WIDTH  / 2) * dX) * coords_begin->zoom;
            float y0 = coords_begin->y_shift + (((float)iy - SCREEN_HEIGHT / 2) * dY) * coords_begin->zoom * HEIGHT_WIDTH_RELATION;

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
               pixel_array[iy * SCREEN_WIDTH + ix + i] = iter_array[i];
            }
        }
    }
}
