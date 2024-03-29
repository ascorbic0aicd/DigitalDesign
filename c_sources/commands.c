#include "help.h"
#include "echo.h"
#include "hello.h"
#include "author.h"
#include "setColor.h"
#include "clear.h"
#include "getTime.h"
#include "Qstring.h"
#include "commands.h"
#include "fib.h"
#include "sys.h"
#include "eval.h"
#include "snake.h"
#include "tetris.h"
#include "bird.h"
#include "bench.h"
#include "type.h"

Func funcs[NUM_OF_FUNC] = {{"hello" ,hello}, {"echo", echo}, {"fib", fib},{"help", help},{"author",author},
                            {"setcolor",setColor_in_cmd},{"clear",clear},{"time",getTime}, {"eval", eval}, {"snake", snake},
                            {"tetris", tetris}, {"bird", bird},{"benchmark", benchmark},{"typeing-game",typeing_game}};

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


