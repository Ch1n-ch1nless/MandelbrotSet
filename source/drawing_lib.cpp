#include "drawing_lib.h"

Picture* PictureCtor(float x_shift, float y_shift, float zoom)
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

void PictureDtor(Picture* picture)
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

void DrawWindow(Picture* picture, void (*CalculateMandelbrotSet)(Picture* picture))
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot_set", sf::Style::Default);

    while(window.isOpen())
    {
        sf::Event event;
        
        if (UpdateUserSettings(picture) == CLOSE_WINDOW) break;

        //time_start
        CalculateMandelbrotSet(picture);
        //time_end

        picture->texture.update((const uint8_t *) (picture->pixel_array));

        window.clear();

        window.draw(picture->sprite);

        window.display();
    }

}

int UpdateUserSettings(Picture* picture)
{
    assert((picture != nullptr) && "Pointer to \'picture\' is NULL!!!\n");

    sf::Keyboard keyboard;

    if (keyboard.isKeyPressed(sf::Keyboard::Add))
    {
        picture->zoom /= 1.1f;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Subtract))
    {
        picture->zoom *= 1.1f;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Up))
    {
        picture->y_shift -= 0.1f;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Down))
    {
        picture->y_shift += 0.1f;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Left))
    {
        picture->x_shift -= 0.1f;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Right))
    {
        picture->x_shift += 0.1f;
    }
    if (keyboard.isKeyPressed(sf::Keyboard::Escape))
    {
        return CLOSE_WINDOW;
    }

    return NO_COMMANDS;
}

void SetPixel(unsigned int* pixel_array, int x_pos, int y_pos, int iter_quantity)
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
