#include <settings.h>

void BaseUtilities::print(char *format, ...)
{
    if (PRINT_ENABLED)
    {
        va_list args;

        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}