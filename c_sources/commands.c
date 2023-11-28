#include "Qstring.h"
#include "echo.h"
#include "hello.h"
#include "commands.h"
#include "fib.h"
#include "sys.h"
Func funcs[NUM_OF_FUNC] = {{"hello" ,hello}, {"echo", echo}, {"fib", fib}};

func_ptr findFunc(char *name)
{
    for (int i = 0; i < NUM_OF_FUNC; i++)
    {
        if (Qstrcmp(name, funcs[i].name) == 0)
        {
            return funcs[i].ptr;
        }
    }

    return NULL;
}


