#include "sys.h"
#include "commands.h"
#include "Qstring.h"
void Qash()
{
    
    int arg;
    char buff[1500];

    while (1)
    {
        color c = getColor();
        setColour(GREEN);
        puts("qxb&cry&lyp@verilog");
        setColour(WHITE);
        puts(":");
        setColour(BLUE);
        puts("~");
        setColour(WHITE);
        puts("$ ");
        setColour(c);
        getbuff(buff);
        char args[20][50];
        arg = checkBuff(buff, args);
        if (arg != 114 && arg != 0)
        {
            exec(arg, args);            
            putchar('\n');
        }
    }
    
    
    
    
}

void getbuff(char buff[])
{
    char c = 0;
    int i = 0;
    while (1)
    {
        c = getchar();
        buff[i] = c;
        if (c == BACKSPACE)
        {
            if (i != 0)
            {
                putchar(c);
                i--;
                buff[i] = 0;
            }
        }
        else
        {
            putchar(c);
            if (c == '\n')
            {
                break;
            }
            ++i;
        }
        
    }
    buff[i] = '\0';
    
}
int checkBuff(char buff[],char args[][50])
{
    int i = 0;
    int arg = 0;
    int j = 0;
    while (buff[i + j] != '\0')
    {
        if (buff[i + j]!= ' ')
        {
            int k = 0;
            while (buff[i + j] != ' ' && buff[i + j] != '\0' && k < 50)
            {
                args[arg][k] = buff[i + j];
                j++;
                k++;
            }
            if (k >= 50)
            {   
                puts("The arg is too long!\n");
                return 114;
            }
            
            args[arg][k] = '\0';

            if (arg == 10)
            {
                puts("There are to many args!\n");
                return 114;
            }
            arg++;
            if (buff[i + j] == '\0')
            {
                break;
            }
            
        }
        i++;

    }
    return arg;
}
void exec(int arg, char args[][50])
{

    func_ptr p = findFunc(args[0]);
    if (p==NULL)
    {
        puts("Command Not Found!");
    }
    else
    {
        p(arg, args);
    }
    
}