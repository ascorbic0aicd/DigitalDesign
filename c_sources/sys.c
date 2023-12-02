#include "sys.h"
#include "Qstring.h"
#ifdef RV32

char *color_start = (char *)COLOR_REG;
char global_color;

char* vga_start = (char*) VGA_START;
int* line_NO = (int*)VGA_LINE_NO;
int vga_line = 0;
int vga_ch = 0;
int line_d = 0;
char need_plus_off = 0;
unsigned char ends[32];
#define true_y ((line_d + vga_line) & VGA_MAXREG)
void vga_init()
{
    vga_line = 0;
    line_d = 0;
    vga_ch = 1;

    (*line_NO) = 0;
    need_plus_off = 0;
    for (int i = 0; i < VGA_MAXCOL; i++)
    {
        for(int j = 0; j < VGA_MAXREG; j++)
        {
            vga_start[ (i<<5) + j ] = 0;
        }       
    }
}
void vga_clear_line(int y)
{
    for (int i = 0; i < VGA_MAXCOL; i++)
    {
        vga_start[(i<<5) + y] = 0; 
    }
    vga_ch = 1;
}
void putchar(const char ch) 
{

    if(ch == 8) //backspace
    {
        vga_ch--;
        vga_start[(vga_ch << 5) + vga_line] = 0;
        if (vga_ch == 0)
        {
            vga_line--;
            vga_ch = ends[vga_line + vga_line];
            
        }
        
        return;
    }
    if(ch == 10) //enter
    {
        //TODO
        ends[vga_line] = vga_ch;
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
        return;
    }
    vga_start[(vga_ch << 5) + vga_line] = ch;
    color_start[(vga_ch << 5) + vga_line] = global_color; 
    vga_ch++;
    if(vga_ch>=VGA_MAXCOL)
    {
        //TODO
        ends[true_y] = VGA_MAXCOL;
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
    }
    return;
}
void vga_rollLine()
{
    if (vga_line >= VGA_MAXLINE)
    {
        need_plus_off = 1;
    }
    if (need_plus_off)
    {
        line_d++;
        line_d &= VGA_MAXREG;
        vga_line &= VGA_MAXREG;
        vga_clear_line(vga_line);
        (*line_NO) = line_d;
    }
    

}
void puts(const char *str)
{
    for(char* p=str;*p != 0;p++)
        putchar(*p);
}
char getColor()
{
    return global_color;
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
void setColour(color c)
{
    global_color = c;
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