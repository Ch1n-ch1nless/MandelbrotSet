#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>

/*==================CONSTANTS=================*/

const int   MAX_NUMBER_OF_ITERATIONS = 256;

const int   SCREEN_WIDTH  = 800;
const int   SCREEN_HEIGHT = 600;

const float MAX_SQUARE_RADIUS = 100.0f;
const float dX = 1.0f/400.f;
const float dY = 1.0f/300.f;

const unsigned BLACK = 0xFF000000;

struct picture
{
    sf::Texture texture; 
    sf::Sprite  sprite;
    float x_shift = 0;
    float y_shift = 0;
    float zoom    = 1;
};

/*============================================*/

/*============FUNCTION_DECLARATION============*/

void CountMandelBrotSet(unsigned* const pixel_array, const float center_x, const float center_y, const float zoom);

void SetPixel(unsigned int* const pixel_array, const int x_pos, const int y_pos, const int iter_quantity)
{
    assert((pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    if (iter_quantity == MAX_NUMBER_OF_ITERATIONS)
    {
        *(pixel_array + (y_pos * SCREEN_WIDTH + x_pos)) = BLACK;
    }
    else
    {
        unsigned char coef = (unsigned char)(sqrtf(sqrtf((float)iter_quantity / (float)MAX_NUMBER_OF_ITERATIONS)) * 255.f);
        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 0) = 255 - coef;
        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 1) = 64 * (coef % 2);
        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 2) = coef;
        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 3) = 0xFF;
    }
}

/*============================================*/

int main()
{
    float center_x = -0.75f;
    float center_y =  0.0f;
    float zoom     =  1.0f;

    unsigned int* pixel_array = (unsigned int*) calloc(SCREEN_HEIGHT * SCREEN_WIDTH, sizeof(int));

    picture mandelbrot_set = {};
    mandelbrot_set.texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    mandelbrot_set.sprite.setTexture(mandelbrot_set.texture);

    CountMandelBrotSet(pixel_array, center_x, center_y, zoom);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot_set", sf::Style::Default);

    while(window.isOpen())
    {
        mandelbrot_set.texture.update((const uint8_t *) pixel_array);

        window.clear();

        window.draw(mandelbrot_set.sprite);

        window.display();
    }

    free(pixel_array);

    return 0;
}

void CountMandelBrotSet(unsigned int* const pixel_array, const float center_x, const float center_y, const float zoom)
{
    assert((pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    for (int iy = 0; iy < SCREEN_HEIGHT; iy++)
    {
        for (int ix = 0; ix < SCREEN_WIDTH; ix++)
        {
            float x0 = (((float)ix - SCREEN_WIDTH  / 2) * dX + center_x) * zoom;
            float y0 = (((float)iy - SCREEN_HEIGHT / 2) * dY + center_y) * zoom;

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

            SetPixel(pixel_array, ix, iy, number_of_iterations);
        }
    }
}