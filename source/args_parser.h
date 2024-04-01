#ifndef ARGS_PARSER_H
#define ARGS_PARSER_H

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX(a, b) (a > b) ? a : b 
#define MIN(a, b) (a < b) ? a : b 

/*==================CONSTANTS=================*/

const char* const OPTION_HELP           = "-h";
const char* const OPTION_TEST           = "-t";
const char* const OPTION_GRAPH          = "-g";
const char* const OPTION_IMPLEMENTATION = "-i";

const char* const STRING_SIMPLE         = "simple";
const char* const STRING_VECTOR         = "vector";
const char* const STRING_AVX            = "avx";

enum Mode : int
{
    MODE_UNDEFINED  = -1,
    MODE_TEST       =  0,
    MODE_GRAPH      =  1,
    MODE_SHOW_HELP  =  2,
};

enum Implementation
{
    IMP_UNDEFINED       = -1,
    IMP_SIMPLE          =  0,
    IMP_VECTOR_NO_AVX   =  1,
    IMP_AVX             =  2,
};

/*===================STRUCTS==================*/

struct LaunchSettings
{
    Mode            mode           = MODE_UNDEFINED;
    Implementation  implementation = IMP_UNDEFINED;
};

/*============FUNCTION_DECLARATION============*/

LaunchSettings  ParseArguments(const int argc, const char** const argv);
void            ShowHelp(void);

/*============================================*/

#endif