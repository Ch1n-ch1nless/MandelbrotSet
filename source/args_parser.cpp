#include "args_parser.h"

void ShowHelp(void)
{
    printf( "Guide, how to correct launch the program\n"
            "Options:\n"
            "# \'-h\' -- show guide\n"
            "# \'-t\' -- run the program in test mode\n"
            "# \'-g\' -- run the program with graphic interface\n"
            "\nChoose the implementation\n"
            "# \'-i <implementation>\' -- run the program with this implementation of function: \'CalculateMandelbrot\'\n"
            "\tImplementations:\n"
            "\t@ \'simple\' -- program calculate the set by 1 point\n"
            "\t@ \'vector\' -- program calculate the set by array of 8 points\n"
            "\t@ \'avx\'    -- program calculate the set by SIMD instructions\n"
            "\nExamples:\n"
            "1) ./mandelbrot_set -g -i avx\n"
            "2) ./mandelbrot_set -i simple -t\n\n");
}

LaunchSettings ParseArguments(const int argc, const char** const argv)
{
    assert((argv != nullptr) && "Pointer to \'argv\' is NULL!!!\n");
    assert((argc >  0)       && "Incorrect value of \'argc\'!!!\n");

    LaunchSettings settings = {};
    
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], OPTION_HELP) == 0)
        {
            settings.mode = MODE_SHOW_HELP;
        }
        else if (strcmp(argv[i], OPTION_TEST) == 0)
        {
            settings.mode = MAX(settings.mode, MODE_TEST);
        }
        else if (strcmp(argv[i], OPTION_GRAPH) == 0)
        {
            settings.mode = MAX(settings.mode, MODE_GRAPH);
        }
        else if (i != (argc - 1) && strcmp(argv[i], OPTION_IMPLEMENTATION) == 0)
        {
            ++i;
            if (strcmp(argv[i], STRING_SIMPLE) == 0)
            {
                settings.implementation = IMP_SIMPLE;
            }
            else if (strcmp(argv[i], STRING_VECTOR) == 0)
            {
                settings.implementation = IMP_VECTOR_NO_AVX;
            }
            else if (strcmp(argv[i], STRING_AVX) == 0)
            {
                settings.implementation = IMP_AVX;
            }
            else 
            {
                break;
            }
        }
    }

    return settings;
}

