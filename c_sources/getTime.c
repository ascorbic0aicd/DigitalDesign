#include "sys.h"
#include "Qstring.h"
void getTime(int arg, char args[][50])
{
    if (arg != 1)
    {
        puts("There are too many args!");
        return;
    }
    unsigned res = time(0);
    char data[20];
    res /= 1000;
    Qitos(data,res);
    puts(data);    
    puts(" sec has passed since the machine started.");
}