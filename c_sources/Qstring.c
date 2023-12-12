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
void Qdtos(char *str, double Data, int n) // n代表小数点后个数
{
    int flag = 0;
    double data = Data;
    if (data < 0)
    {
        flag = 1;
        data = -data;
    }
    for (int i = 0; i < n; i++)
        data = data * 10;

    if (data == 0)
    {
        n = n + 2;
        str[n] = '\0';
        n--;
        while (n >= 0)
        {
            str[n] = '0';
            n--;
        }
        str[1] = '.';
        return;
    }
    // 计算最后应该输出的字符串长度（含小数点）
    int count = 1 + n;
    unsigned int d = data;
    unsigned int temp = Data;
    while (temp > 0)
    {
        count++;
        temp /= 10;
    }
    if (flag)
    {
        count++;
    }
    if (Data < 1)
    {
        count++;
    }
    // 开始修改字符串内存
    // 小数点后的部分
    str[count] = '\0';
    count--;
    while (n > 0)
    {
        temp = d % 10;
        str[count] = temp + '0';
        count--;
        n--;
        d /= 10;
    }
    // 小数点
    str[count] = '.';
    count--;
    // 小数点前的部分
    while (d > 0)
    {
        temp = d % 10;
        str[count] = temp + '0';
        count--;
        d /= 10;
    }
    if (Data < 1)
    {
        str[count] = '0';
        count--;
    }
    if (flag)
    {
        str[count] = '-';
    }
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