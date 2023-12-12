#include "sys.h"
#include "Qrand.h"

#define MaxRandom 32768
int TYPENUM = 0;     // 要求打字的总个数
int typenum = 0;     // 当前输入字符数量
int TYPETIME = 0; // 打字总时间
int RIGHTNUM = 0;    // 打字错误个数


void type_show()
{
    char str[33];
    puts("Time(ms):");
    Qitos(str, TYPETIME);
    puts(str);
    puts("\n");
    puts("Accuracy:");
    Qdtos(str, (double)RIGHTNUM / typenum, 2);
    puts(str);
    puts("\n");
    puts("Speed(keys/minute):");
    Qitos(str, typenum * 1000 * 60 / TYPETIME);
    puts(str);
    puts("\n");
    puts("Word count:");
    Qitos(str, TYPENUM);
    puts(str);
    puts("\n");
    puts("Correct word count:");
    Qitos(str, RIGHTNUM);
    puts(str);
    puts("\n");
    puts("Wrong word count:");
    Qitos(str, TYPENUM - RIGHTNUM);
    puts(str);
    puts("\n");
}

void typeing_game(int arg, char args[][50])
{
    char str[100];
    if (arg != 1)
    {
        puts("There are too many args!");
        return;
    }
    else
    {
        TYPENUM = 0;            // 要求打字的总个数
        typenum = 0;            // 当前输入字符数量
        TYPETIME = 0;        // 打字总时间
        RIGHTNUM = 0;           // 打字错误个数

        TYPENUM = Qrand() % 11 + 10; // 范围10-20个
        for (int i = 0; i < TYPENUM; i++)
        {
            int type = Qrand() % 2;
            if (type == 0)
                str[i] = Qrand() % 26 + 65; // 大写字母
            else
                str[i] = Qrand() % 26 + 97; // 小写字母
        }
        str[TYPENUM] = '\0';
        puts(str);
        puts("\n");

        long long int time1 = time(0);
        char c;          // 当前输入字符
        char str1[100];   // 当前输入字符串
        while (1)
        {
            c = getchar();
            if(c == 10) {
                putchar(c);
                break;
            }
            /*else if(c == 8)
            {
                typenum--;
                str1[typenum] = '\0';
            }*/
            else if (c != 0)
            {
                str1[typenum] = c;
                typenum++;
                putchar(c);
            }
        }
        long long int time2 = time(0);

        TYPETIME = time2 - time1;
        for (int i = 0; i < typenum; i++)
        {
            if (str[i] == str1[i] && i < TYPENUM)
                RIGHTNUM++;
        }

        type_show();
        return;
    }
}