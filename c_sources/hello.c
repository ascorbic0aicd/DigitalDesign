#include "sys.h"
void hello(int arg, char args[][50])
{
    if (arg > 1)
    {
        puts("There are to many args!");
    }
    else
    {
        puts("Hello world!");
    }
}