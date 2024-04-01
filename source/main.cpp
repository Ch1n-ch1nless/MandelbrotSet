#include "drawing_lib.h"
#include "args_parser.h"

void TestFunction(void (*CalculateMandelbrotSet)(unsigned int* pixel_array, Coords* coords_begin))
{
    unsigned int* pixel_array = (unsigned int*) calloc(SCREEN_HEIGHT * SCREEN_WIDTH, sizeof(unsigned int));
    assert((pixel_array != nullptr) && "Program can not allocate memory!\n");
    Coords coords_begin = {};

    for (int i = 0; i < 100; i++)
    {
        unsigned long long time_begin   = __rdtsc();
        CalculateMandelbrotSet(pixel_array, &coords_begin);
        unsigned long long time_end     = __rdtsc();
        printf("%lld\n", time_end - time_begin);
    }

    free(pixel_array);
}

int main(int argc, const char* argv[])
{
    LaunchSettings settings = ParseArguments(argc, argv);

    printf("Settings: %d %d\n", settings.mode, settings.implementation);

    switch (settings.mode)
    {
        case MODE_SHOW_HELP:
        {
            ShowHelp();
            break;
        }

        case MODE_GRAPH:
        {
            Picture* picture = PictureCtor(X_SHIFT, Y_SHIFT, ZOOM_DEFAULT);

            switch (settings.implementation)
            {
            case IMP_SIMPLE:
                DrawWindow(picture, PerPixelCalculateMandelbrotSet);
                break;
            
            case IMP_AVX:
                DrawWindow(picture, IntrinsicCalculateMandelbrotSet);
                break;

            case IMP_VECTOR_NO_AVX:
                DrawWindow(picture, VectorCalculateMandelbrotSet);
                break;
            
            case IMP_UNDEFINED:
            default:
                printf("ERROR! Please, read the guide!!!\n");
                ShowHelp();
                break;
            }

            PictureDtor(picture);
            break;
        }

        case MODE_TEST:
        {
            switch (settings.implementation)
            {
            case IMP_SIMPLE:
            {
                TestFunction(PerPixelCalculateMandelbrotSet);
                break;
            }
            
            case IMP_AVX:
            {
                TestFunction(IntrinsicCalculateMandelbrotSet);
                break;
            }

            case IMP_VECTOR_NO_AVX:
            {
                TestFunction(VectorCalculateMandelbrotSet);
                break;
            }
            
            case IMP_UNDEFINED:
            default:
                printf("ERROR! Please, read the guide!!!\n");
                ShowHelp();
                break;
            }
            break;
        }
        
        case MODE_UNDEFINED:
        default:
        {
            printf("ERROR! Please, read the guide!!!\n");
            ShowHelp();
            break;
        }
    }

    return 0;
}

