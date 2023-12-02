#include "sys.h"
#include "help.h"
#include "commands.h"
extern Func funcs[NUM_OF_FUNC];
void help(int arg, char args[][50])
{
    if (arg != 1)
    {
        puts("There are to many args");
        return;
    }

    for (int i = 0; i < NUM_OF_FUNC; i++)
    {
        puts(funcs[i].name);
        putchar(' ');
    }
        
}