//const int STACK_SIZE = 100;
typedef int StaDataType;
typedef struct 
{
    int top;
    StaDataType data[100];
    int STACK_SIZE;
} Stack;

void Init(Stack *s);
void Push(Stack *s, int i);
void Pop(Stack *s, int *i);
void makeEmpty(Stack *s);
int Size(Stack *s);
int Empty(Stack *s);
StaDataType Top(Stack * s);
void stackprint(Stack *s);