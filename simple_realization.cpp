#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>

/*==================CONSTANTS=================*/

const int   MAX_NUMBER_OF_ITERATIONS = 256;

const int   SCREEN_WIDTH  = 800;
const int   SCREEN_HEIGHT = 600;

const float MAX_SQUARE_RADIUS   =  100.0f;
const float dX                  =  1.0f/400.f;
const float dY                  =  1.0f/300.f;
const float X_SHIFT             = -0.75f;
const float Y_SHIFT             =  0.0f;
const float ZOOM                =  1.0f;

const unsigned BLACK = 0xFF000000;

struct Picture
{
    sf::Texture     texture; 
    sf::Sprite      sprite;
    float           x_shift     =  0.0f;
    float           y_shift     =  0.0f;
    float           zoom        =  0.0f;
    unsigned int*   pixel_array =  nullptr;
};

/*============================================*/

/*============FUNCTION_DECLARATION============*/

Picture*    PictureCtor(const float x_shift, const float y_shift, const float zoom);
void        PictureDtor(Picture* const picture);

void        DrawWindow (Picture* const picture);
void        DrawWindow2(Picture &picture);

void CountMandelbrotSet(unsigned* const pixel_array, const float center_x, const float center_y, const float zoom);

void SetPixel(unsigned int* const pixel_array, const int x_pos, const int y_pos, const int iter_quantity);

/*============================================*/

int main()
{
    Picture* mandelbrot_picture = PictureCtor(X_SHIFT, Y_SHIFT, ZOOM);

    DrawWindow(mandelbrot_picture);

    PictureDtor(mandelbrot_picture);

    return 0;
}

/*===========================================================*/

Picture* PictureCtor(const float x_shift, const float y_shift, const float zoom)
{
    Picture* picture = new Picture;

    picture->x_shift = x_shift;
    picture->y_shift = y_shift;
    picture->zoom    = zoom;

    if (!picture->texture.create(SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        assert(0 && "Not create!\n");
    }
    
    picture->sprite.setTexture(picture->texture);

    picture->pixel_array = (unsigned int*) calloc(SCREEN_HEIGHT * SCREEN_WIDTH, sizeof(unsigned int));
    assert((picture->pixel_array != nullptr) && "Program can not allocate memory!\n");

    return picture;
}   

void PictureDtor(Picture* const picture)
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    picture->x_shift = 0.0f;
    picture->y_shift = 0.0f;
    picture->zoom    = 0.0f;

    free(picture->pixel_array);
    picture->pixel_array = nullptr;

    free(picture);

    return;
}

void DrawWindow(Picture* const picture)
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot_set", sf::Style::Default);

    while(window.isOpen())
    {
        //time_start
        CountMandelbrotSet(picture->pixel_array, picture->x_shift, picture->y_shift, picture->zoom);
        //time_end

        ((*picture).texture).update((const uint8_t *) (picture->pixel_array));

        window.clear();

        window.draw(picture->sprite);

        window.display();
    }

}

/*===========================================================*/

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

void CountMandelbrotSet(unsigned int* const pixel_array, const float center_x, const float center_y, const float zoom)
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