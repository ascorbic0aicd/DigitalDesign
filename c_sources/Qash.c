#include "sys.h"
#include "commands.h"

void Qash()
{
    
    int arg;
    char buff[1500];

    while (1)
    {
        puts("qxb&cry@verillog:~$ ");
        getbuff(buff);
        char args[10][50];
        arg = checkBuff(buff, args);
        if (arg != 114 || arg != 0)
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

        putchar(c);
        
        buff[i] = c;
        if (c == '\n')
        {
            break;
        }
        ++i;
    }
    buff[i] = '\0';
    
}
int checkBuff(char buff[],char args[][50])
{
    int i = 0;
    int arg = 0;
    int j = 0;
    while (buff[i] != '\0')
    {
        
        int k = 0;
        while (buff[i + j] != ' ' && buff[i + j] != '\0')
        {
            //printf("i = %d, j = %d c = %c %d\n", i, j, buff[i + j], buff[i + j]);
            args[arg][k] = buff[i + j];
            j++;
            k++;
        }
        args[arg][j] = '\0';
        //printf("args[%d] =  %s\n",arg,args[arg]);
        arg++;

        if (arg == 10)
        {
            puts("There are to many args!");
            return 114;
        }
        if (buff[i + j] == '\0')
        {
            break;
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