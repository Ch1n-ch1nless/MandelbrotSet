#ifndef DRAWING_LIBRARY
#define DRAWING_LIBRARY

#include "config.h"
#include <SFML/Graphics.hpp>

/*==================CONSTANTS=================*/

const unsigned  BLACK_COLOR = 0xFF000000;

/*===================STRUCTS==================*/

struct Picture
{
    sf::Texture     texture; 
    sf::Sprite      sprite;
    Coords          coords;
    unsigned int*   pixel_array =  nullptr;
};

struct FPS
{
    sf::Text    text;
    sf::Font    font;
    sf::Clock   clock;
    sf::Time    time;
};

/*============FUNCTION_DECLARATION============*/

Picture*    PictureCtor(float x_shift, float y_shift, float zoom);
void        PictureDtor(Picture* picture);

FPS*        FPSCtor(void);
void        FPSDtor(FPS* program_fps);

void        WriteFPS(FPS* program_fps, sf::RenderWindow &window);

void        DrawWindow (Picture* picture, void (*CalculateMandelbrotSet)(unsigned int* pixel_array, int x_shift, int y_shift, int zoom));

int         UpdateUserSettings(Picture* picture);

void        SetPixelArray(unsigned int* pixel_array);
void        SetPixel(unsigned int* pixel_array, int x_pos, int y_pos, int iter_quantity);

/*============================================*/

#endif 