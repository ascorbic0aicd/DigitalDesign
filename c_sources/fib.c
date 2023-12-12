#include "sys.h"
#include "Qstring.h"
void fib(int arg, char args[][50])
{
    char str[22];
    if (arg != 2)
    {
        if (arg == 1)
        {
            puts("There are too few args!");
        }
        else
        {
            puts("There are too many args!");
        }
        return;
    }
    else
    {
        
        int a = 0, b = 1;
        int n = Qstoi(args[1]);

            
        int temp = 0;
        if (n==NAN)
        {
            return;
        }
        
        if (n == 0 || n == 1)
        {
            Qitos(str,n);
            puts(str);
            return;
        }
        else if (n < 0)
        {
            puts("n < 0!");
            
            return;
        }
        
        while (n >= 2)
        {
            temp = a + b;
            a = b;
            b = temp;
            n--;
        }
        Qitos(str,temp);
        puts(str);
        return;
    }
}