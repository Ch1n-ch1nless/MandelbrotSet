#ifndef DRAWING_LIBRARY
#define DRAWING_LIBRARY

#include <assert.h>
#include <immintrin.h>
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
const float ZOOM_DEFAULT        =  1.0f;

const float SHIFT_CONSTANT      = 0.1f;
const float ZOOM_CONSTANT       = 1.1f;

const unsigned BLACK = 0xFF000000;

enum UserCommands : int
{
    NO_COMMANDS     = 0,
    CLOSE_WINDOW    = 1,
};

struct Picture
{
    sf::Texture     texture; 
    sf::Sprite      sprite;
    float           x_shift     =  0.0f;
    float           y_shift     =  0.0f;
    float           zoom        =  0.0f;
    unsigned int*   pixel_array =  nullptr;
};

struct FPS
{
    sf::Text    text;
    sf::Font    font;
    sf::Clock   clock;
    sf::Time    time;
};

/*============================================*/

/*============FUNCTION_DECLARATION============*/

Picture*    PictureCtor(float x_shift, float y_shift, float zoom);
void        PictureDtor(Picture* picture);

FPS*        FPSCtor(void);
void        FPSDtor(FPS* program_fps);

void        WriteFPS(FPS* program_fps, sf::RenderWindow &window);

void        DrawWindow (Picture* picture, void (*CalculateMandelbrotSet)(Picture* picture));

int         UpdateUserSettings(Picture* picture);

void        SetPixel(unsigned int* pixel_array, int x_pos, int y_pos, int iter_quantity);

/*============================================*/

#endif 