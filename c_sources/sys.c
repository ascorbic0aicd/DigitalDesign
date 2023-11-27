#include "sys.h"
char* vga_start = (char*) VGA_START;
int vga_line=0;
int vga_ch=0;

void vga_init(){
    vga_line = 0;
    vga_ch =0;
        for(int j=0;j<5;j++)
            vga_start[ (j<<5) + 0 ] = 0x61;
}

void putch(char ch) 
{
    if(ch==8) //backspace
    {
        //TODO
        return;
    }
    if(ch==10) //enter
    {
        //TODO
        return;
    }
    vga_start[ (vga_ch<<5)+vga_line] = ch;
    vga_ch++;
    if(vga_ch>=VGA_MAXCOL)
    {
        //TODO
        vga_ch = 0;
        vga_line++;
    }
    return;
}

void putstr(char *str)
{
    for(char* p=str;*p!=0;p++)
        putch(*p);
}