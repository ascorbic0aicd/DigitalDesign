#include "sys.h"
#include "Qstring.h"
int Qstrcmp(const char* str1, const char* str2)
{
    int i = 0;
    for(; str1[i]!='\0' && str2[i]!='\0' && str1[i] == str2[i];++i)
    {
    }
    return str1[i] - str2[i];
}

int Qstrlen(const char* str)
{
    int i = 0;
    for(;str[i] != '\0';++i);
    return i;
}
void Qitos(char *str, int data)
{
    int flag = 0;
    
    if (data < 0)
    {
        flag = 1;
        data = -data;
    }
    unsigned d = data;
    int n = 0;
    unsigned temp = data;
   
    if (data == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    while (temp > 0)
    {
        n++;
        temp /= 10;
    }
    if (flag)
    {
        n++;
    }
    str[n] = '\0';
    n--;
    while (d > 0)
    {
        temp = d % 10;
        str[n] = temp + '0';
        n--;
        d /= 10;
    }
    if (flag)
    {
        str[0] = '-';
    }
}
int Qstoi(const char *str)
{
    int res = 0;
    int len = Qstrlen(str);
    int flag = 0;
    int i = 0;
    if (str[0] == '-')
    {
        flag = 1;
        ++i;
    }
    for(; i<len; ++i)
    {
        res *= 10;
        if (str[i] > '9' || str[i] < '0')
        {
            puts("this is not a number!");
            return NAN;
        }
        res += str[i] - '0';
    }
    if (flag)
    {
        res = - res;
    }
    
    return res;
}