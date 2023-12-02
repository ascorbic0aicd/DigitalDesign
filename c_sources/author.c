#include "sys.h"
void author(int arg, char args[][50])
{
    if (arg != 1)
    {
        puts("There are to many args");
        return;
    }

    puts("This computer was made by qxb & cry & lyp!");
    return;
}