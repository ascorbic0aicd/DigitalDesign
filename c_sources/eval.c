//#include "sys.h"
#include "Qstring.h"
#include "Cmalloc.h"
#include "eval.h"
#include "stack.h"
int Expr_Error=1;
int isCzs(unsigned ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return 1;
    }
    else if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='%' || ch=='^' || ch=='~' || ch=='(' || ch==')' || ch=='#')
    {
        return 0;
    }
    else{
        return 2;
    }
    
}
int isp(unsigned ch)
{
    switch (ch)
    {
    case '(':
        return 1;
        break;
    case '~':
        return 8;
        break;
    case '^':
        return 7;
        break;
    case '*':
    case '/':
    case '%':
        return 5;
        break;
    case '+':
    case '-':
        return 3;
        break;
    case ')':
        return 9;
        break;
    default:
        return 0;
    }
}
int icp(unsigned ch)
{
    switch (ch)
    {
    case '(':
        return 10;
        break;
    case '~':
        return 9;
        break;
    case '^':
        return 6;
        break;
    case '*':
    case '/':
    case '%':
        return 4;
        break;
    case '+':
    case '-':
        return 2;
        break;
    case ')':
        return 1;
        break;
    default:
        return 0;
    }
}

void DoOperator(unsigned x, Stack* czs)
{ // 计算
    int left, right, res;
    if (x == '~')
    {
        if (Size(czs) < 1)
        {
            //puts("4_wrong expression!");
            Expr_Error = 0;
            return;
        }
        else{
            Pop(czs, &right);
            res = 0 - right;
            Push(czs,res);
        }
    }
    else
    {
        if(Size(czs)<2){
            //puts("3_wrong expression!");
            Expr_Error = 0;
            return;
        }
        else{
            Pop(czs, &right);
            Pop(czs, &left);
        }
        
        if (x == '^')
        {
            res = 1;
            for(int i=0;i<right;i++){
                res = res * left;
            }
        }
        else if (x == '*')
        {
            res = left * right;
        }
        else if (x == '/')
        {
            if(right == 0){
                Expr_Error = 0;
                return;
            }
            res = left / right;
        }
        else if (x == '%')
        {
            if(right == 0){
                Expr_Error = 0;
                return;
            }
            res = left % right;
        }
        else if (x == '+')
        {
            res = left + right;
        }
        else if (x == '-')
        {
            res = left - right;
        }

        Push(czs, res);
    }
}

void EvaluateExpression(char *expr)
{
    //Stack OPTR, OPND;

    Stack *OPTR = (Stack *)Cmalloc(sizeof(Stack));
    Stack *OPND = (Stack *)Cmalloc(sizeof(Stack));
    Init(OPTR);
    Init(OPND);
    Push(OPTR, '#');
    int i = 0;
    unsigned ch = expr[i];
    unsigned theta, temp;
    int a, b;
    while (ch != '\0' && Expr_Error)
    {
        // puts("czf");
        // stackprint(&OPTR);
        // puts("czs");
        // stackprint(&OPND);
        if (isCzs(ch) == 1) // 操作数
        {
            int ope = ch - '0';
            i++;
            ch = expr[i];
            while(isCzs(ch)==1){
                ope = ope * 10 + ch - '0';
                i++;
                ch = expr[i];
            }
            Push(OPND, ope);
        }
        else if (isCzs(ch) == 0)
        {
            int s = isp(Top(OPTR));
            int c = icp(ch);
            if (s < c)
            {
                Push(OPTR, ch);
                i++;
                ch = expr[i];
            }
            else if (s > c)
            {
                Pop(OPTR, &theta);
                DoOperator(theta, OPND);
            }
            else{
                Pop(OPTR, &temp);
                if(temp=='('){
                    i++;
                    ch = expr[i];
                }
                else{
                    DoOperator(temp, OPND);
                }
            }
        }
        else{
            //puts("2_wrong expression!");
            Expr_Error = 0;
        }
    }

    if (Expr_Error == 0)
    {
        puts("wrong expression!");
        return;
    }
    else if (Expr_Error == 1){
        while (!Empty(OPTR) && Expr_Error)
        {
            // puts("czf");
            // stackprint(&OPTR);
            // puts("czs");
            // stackprint(&OPND);

            Pop(OPTR, &theta);
            if (theta != '#')
            {
                DoOperator(theta, OPND);
            }
            else{
                break;
            }
        }
        if (Expr_Error == 0)
        {
            puts("wrong expression!");
            return;
        }
        else{
            char str[20];
            if(Size(OPND) == 1){
                int x = Top(OPND);
                Qitos(str, x);
                puts(str);
            }
            else{
                //puts("1_wrong expression!");
                Expr_Error = 0;
            }
        }
    }

    if (Expr_Error == 0)
    {
        puts("wrong expression!");
    }
    //makeEmpty(OPND);
    //makeEmpty(OPTR);
    Cfree(OPND);
    Cfree(OPTR);
}



void eval(int arg, char args[][50])
{
    if (arg == 1)
    {
        puts("There are too few args!");
        return;
    }
    
    char expr[10000];
    int k = 0;
    for (int i = 1; i < arg; i++)
    {
        for (int j = 0; j < Qstrlen(args[i]); j++)
        {
            expr[k] = args[i][j];
            k++;
        }
    }
    expr[k] = '\0';
    Expr_Error = 1;
    //puts(expr);
    EvaluateExpression(expr);
    return;
}