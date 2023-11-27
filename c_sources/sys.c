#include "sys.h"
char* vga_start = (char*) VGA_START;
int vga_line = 0;
int vga_ch=0;
unsigned char ends[70];
void vga_init()
{
    vga_line = 0;
    vga_ch = 1;
    for (int i = 0; i < VGA_MAXCOL; i++)
    {
        for(int j=0;j<VGA_MAXLINE;j++)
        {
            vga_start[ (i<<5) + j ] = 0;
        }       
    }
}

void putch(char ch) 
{
    if(ch==8) //backspace
    {
        if (vga_ch == 1)
        {
            vga_line--;
            vga_ch = ends[vga_line];
        }
        else
        {
            vga_start[(vga_ch << 5)+vga_line] = 0;
            vga_ch--;
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

void putstr(char *str)
{
    for(char* p=str;*p!=0;p++)
        putch(*p);
}

char Qgetchar()
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

void Qgets(char *str, int limit)
{
    int cnt = 0;
    char c = 0;
    while (cnt < limit)
    {
        c = Qgetchar();
        if (c != 10)
        {
            str[cnt] = c;
            c++;
        }
        else
        {
            str[cnt] = 0;
            break;
        }
    }
    str[cnt] = 0;
    return;
    
}