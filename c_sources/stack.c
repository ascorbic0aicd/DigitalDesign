#include "stack.h"
#include "sys.h"
void Init(Stack *s)
{
    s->top = -1;
    s->STACK_SIZE = 100;
}

void Push(Stack *s, int i)
{
    if (s->top == s->STACK_SIZE - 1)
    {
        puts("stack overflow");
        return;
    }
    else
    {
        s->top++;
        s->data[s->top] = i;
        return;
    }
}

void Pop(Stack *s, int *i)
{
    if (s->top == -1)
    {
        puts("stack empty");
        return;
    }
    else
    {
        *i = (int) s->data[s->top];
        s->top--;
        return;
    }
}

void makeEmpty(Stack *s)
{
    s->top = -1;
}

int Size(Stack *s)
{
    return s->top+1;
}

int Empty(Stack *s)
{
    if (s->top == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

StaDataType Top(Stack * s){
    if(s->top != -1){
        return s->data[s->top];
    }
    else{
        puts("stack empty");
        return (StaDataType)0;
    }
}

void stackprint(Stack *s){
    if(s->top != -1){
        for(int i = 0; i <= s->top; i++){
            //printf("%d", s->data[i]);
        }
    }
    else{
        puts("stack empty");
        return;
    }
}