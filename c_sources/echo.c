#include "sys.h"

void echo(int arg, char args[][50])
{
    for (int i = 1; i < arg; i++)
    {
        puts(args[i]);
        putchar(' ');
    }
    
}