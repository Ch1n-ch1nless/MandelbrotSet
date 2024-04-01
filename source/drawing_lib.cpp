#include "drawing_lib.h"

Picture* PictureCtor(float x_shift, float y_shift, float zoom)
{
    Picture* picture = new Picture;

    picture->coords.x_shift = x_shift;
    picture->coords.y_shift = y_shift;
    picture->coords.zoom    = zoom;

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

    picture->coords.x_shift = 0.0f;
    picture->coords.y_shift = 0.0f;
    picture->coords.zoom    = 0.0f;

    free(picture->pixel_array);
    picture->pixel_array = nullptr;

    delete picture;

    return;
}

void DrawWindow(Picture* picture, void (*CalculateMandelbrotSet)(unsigned int* pixel_array, Coords* coords_begin))
{
    assert((picture              != nullptr) && "Pointer to \'picture\' is NULL!!!\n");
    assert((picture->pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Mandelbrot_set", sf::Style::Default);

    FPS* fps = FPSCtor();

    while(window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            sf::Keyboard keyboard;

            if (keyboard.isKeyPressed(sf::Keyboard::Add))
            {
                picture->coords.zoom /= ZOOM_CONSTANT;
            }
            if (keyboard.isKeyPressed(sf::Keyboard::Subtract))
            {
                picture->coords.zoom *= ZOOM_CONSTANT;
            }
            if (keyboard.isKeyPressed(sf::Keyboard::Up))
            {
                picture->coords.y_shift -= SHIFT_CONSTANT * picture->coords.zoom;
            }
            if (keyboard.isKeyPressed(sf::Keyboard::Down))
            {
                picture->coords.y_shift += SHIFT_CONSTANT * picture->coords.zoom;
            }
            if (keyboard.isKeyPressed(sf::Keyboard::Left))
            {
                picture->coords.x_shift -= SHIFT_CONSTANT * picture->coords.zoom;
            }
            if (keyboard.isKeyPressed(sf::Keyboard::Right))
            {
                picture->coords.x_shift += SHIFT_CONSTANT * picture->coords.zoom;
            }
            if (keyboard.isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
        }

        CalculateMandelbrotSet(picture->pixel_array, &(picture->coords));
        SetPixelArray(picture->pixel_array);

        picture->texture.update((const uint8_t *) (picture->pixel_array));

        window.clear();

        window.draw(picture->sprite);

        WriteFPS(fps, window);

        window.display();
    }

    FPSDtor(fps);

}

void SetPixelArray(unsigned int* pixel_array)
{
    assert((pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    for (int y_pos = 0; y_pos < SCREEN_HEIGHT; y_pos++)
    {
        for (int x_pos = 0; x_pos < SCREEN_WIDTH; x_pos++)
        {
            SetPixel(pixel_array, x_pos, y_pos, pixel_array[y_pos * SCREEN_WIDTH + x_pos]);
        }
    }
}

void SetPixel(unsigned int* pixel_array, int x_pos, int y_pos, int iter_quantity)
{
    assert((pixel_array != nullptr) && "Pointer to \'pixel_array\' is NULL!!!\n");

    if (iter_quantity == MAX_NUMBER_OF_ITERATIONS)
    {
        *(pixel_array + (y_pos * SCREEN_WIDTH + x_pos)) = BLACK_COLOR;
    }
    else
    {
        unsigned char coef = (unsigned char)(pow(((float)iter_quantity / (float)MAX_NUMBER_OF_ITERATIONS), 0.35f) * 255.f);

        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 0) = 255 - coef;
        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 1) = 64 * (coef % 2);
        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 2) = coef;
        *(((unsigned char *) pixel_array) + (y_pos * SCREEN_WIDTH + x_pos) * sizeof(unsigned) + 3) = 0xFF;
    }
}

FPS* FPSCtor(void)
{
    FPS* new_fps_struct = new FPS;

    sf::Text text("", new_fps_struct->font, 20);
    new_fps_struct->text = text;
    
    new_fps_struct->text.setColor(sf::Color::Green);
    new_fps_struct->text.setPosition(10, 10);

    if (!new_fps_struct->font.loadFromFile("arial.ttf"))
    {
        assert(0 && "Program can not find the file \'arial.ttf\'\n");
    }

    return new_fps_struct;
}

void FPSDtor(FPS* program_fps)
{
    assert((program_fps != nullptr) && "Pointer to \'program_fps\' is NULL!!!\n");

    free(program_fps);
    return;
}

void WriteFPS(FPS* program_fps, sf::RenderWindow &window)
{
    assert((program_fps != nullptr) && "Pointer to \'program_fps\' is NULL!!!\n");

    float delta_time = program_fps->clock.restart().asSeconds();
    float fps = 1 / delta_time;

    std::string fps_string = "FPS: " + std::to_string(fps);
    program_fps->text.setString(fps_string);

    #ifdef SHOW_FPS
    printf("%f\n", fps);
    #endif

    window.draw(program_fps->text);
}
