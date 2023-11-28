#include "sys.h"
#ifdef RV32

char* vga_start = (char*) VGA_START;
int vga_line = 0;
int vga_ch = 0;
unsigned char ends[70];
void vga_init()
{
    vga_line = 0;
    vga_ch = 1;
    
    for (int i = 0; i < VGA_MAXCOL; i++)
    {
        for(int j = 0; j < VGA_MAXLINE; j++)
        {
            vga_start[ (i<<5) + j ] = 0;
        }       
    }
}

void putchar(char ch) 
{
    if(ch == 8) //backspace
    {
        vga_ch--;
        vga_start[(vga_ch << 5) + vga_line] = 0;
        if (vga_ch == 0)
        {
            
            vga_line--;
            vga_ch = ends[vga_line];
        }
        
        return;
    }
    if(ch==10) //enter
    {
        //TODO
        ends[vga_line] = vga_ch;
        vga_ch = 1;
        vga_line++;
        return;
    }
    vga_start[(vga_ch << 5)+vga_line] = ch;
    vga_ch++;
    if(vga_ch>=VGA_MAXCOL)
    {
        //TODO
        ends[vga_line] = VGA_MAXCOL;  
        vga_ch = 1;
        vga_line++;
    }
    return;
}

void puts(char *str)
{
    for(char* p=str;*p != 0;p++)
        putchar(*p);
}

char getchar()
{

    char *p = (char *) KEY_LOC;
    volatile char temp = *p;
    while (temp == 0)
    {
        temp = *p;
        if (temp != 0)
        {
            return temp;
        }
        
    }
    return 0;
}

void gets(char *str)
{
    int cnt = 0;
    char c = 0;
    while (c != '\n')
    {
        c = getchar();
        str[cnt] = c;
        cnt++;
    }
    str[cnt] = 0;
    return;
    
}
unsigned int __mulsi3(unsigned int a, unsigned int b)
{
    unsigned int res = 0;
    while (a)
    {
        if (a & 1)
            res += b;
        a >>= 1;
        b <<= 1;
    }
    return res;
}
unsigned int __umodsi3(unsigned int a, unsigned int b)
{
    unsigned int bit = 1;
    unsigned int res = 0;
    while (b < a && bit && !(b & (1UL << 31)))
    {
        b <<= 1;
        bit <<= 1;
    }
    while (bit)
    {
        if (a >= b)
        {
            a -= b;
            res |= bit;
        }
        bit >>= 1;
        b >>= 1;
    }
    return a;
}
unsigned int __udivsi3(unsigned int a, unsigned int b)
{
    unsigned int bit = 1;
    unsigned int res = 0;
    while (b < a && bit && !(b & (1UL << 31)))
    {
        b <<= 1;
        bit <<= 1;
    }
    while (bit)
    {
        if (a >= b)
        {
            a -= b;
            res |= bit;
        }
        bit >>= 1;
        b >>= 1;
    }
    return res;
}
#endif