
#define NUM_OF_FUNC 14
typedef void (*func_ptr)(int, char [][50]);
typedef struct
{
    char name[30];
    func_ptr ptr;
} Func;

func_ptr findFunc(char *name);
