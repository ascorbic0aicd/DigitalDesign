#include "sys.h"
void clear(int arg, char args[][50])
{
#ifdef RV32
    if (arg != 1)
    {
        puts("There are too many args");
    }
    else
    {
        vga_init();
    }
#endif    
}