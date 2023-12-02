#include "sys.h"
#include "Qstring.h"
char c[8][10] = {"white", "red", "blue", "green", "black", "yellow", "orange", "purple"};
void setColor_in_cmd(int arg, char args[][50])
{
#ifdef RV32
    if (arg!=2)
    {
        if (arg > 2)
        {   
            puts("There are too many args!");
        }
        else
        {
            puts("There are too few args");
        }
        return;
        
    }
    
    if (Qstrcmp(args[1],"-h") == 0)
    {
        puts("You can change color to:red,green,blue,orange,yellow,purple,white,black!");
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        if (Qstrcmp(c[i],args[1]) == 0)
        {
            setColour(i);
            puts("The color is change to ");
            puts(c[i]);
            return;
        }
    }
    puts("invalid arg! You can use -h to see which color you can change to");
#endif
}