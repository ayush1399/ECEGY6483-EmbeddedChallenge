#pragma once
#include <mbed.h>

// #define SETTINGS 0
#define PRINT_ENABLED 1

class BaseUtilities
{
public:
    void print(char *format, ...);
};