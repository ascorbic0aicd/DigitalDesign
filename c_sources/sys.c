#include "sys.h"
#include "Qstring.h"
char *color_start = (char *)COLOR_REG;
char global_color;

char *vga_start = (char *)VGA_START;
int *line_NO = (int *)VGA_LINE_NO;
int vga_line = 0;
int vga_ch = 0;
int line_d = 0;
char need_plus_off = 0;
unsigned char ends[32];
short *vga_cursor_p = (short *)CURSOR_ADDR;

int *timer = (int *)TIMER_ADDR;

bool in_normal = true;
#ifdef RV32
#define true_y ((line_d + vga_line) & VGA_MAXREG)
#define cursor_loc ((vga_ch << 5) + vga_line)
void vga_init()
{
    vga_line = 0;
    line_d = 0;
    vga_ch = 1;
    (*line_NO) = 0;
    (*vga_cursor_p) = 0;
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

    if(ch == BACKSPACE) //backspace
    {
        vga_ch--;
        vga_start[(vga_ch << 5) + vga_line] = 0;
        if (vga_ch == 0)
        {
            if (vga_line)
            {
                vga_line--;
            }
            else
            {
                vga_line = 31;
            }
            vga_ch = ends[vga_line];
        }
        (*vga_cursor_p) = cursor_loc;
        return;
    }
    if(ch == 10) //enter
    {
        ends[vga_line] = vga_ch;
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
        (*vga_cursor_p) = cursor_loc;
        return;
    }

    if (vga_ch == VGA_MAXCOL)
    {
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
    }
    vga_start[(vga_ch << 5) + vga_line] = ch;
    color_start[(vga_ch << 5) + vga_line] = global_color; 
    vga_ch++;
    if(vga_ch>=VGA_MAXCOL)
    {
        ends[vga_line] = VGA_MAXCOL;
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
    }
    (*vga_cursor_p) = cursor_loc;
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
bool kbhit()
{
    char *check_k = (char *)CHECK_KEY;
    char p = *check_k;
    return p != 0 ;
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

int time(int _)
{
    return (*timer);
}
#endif
void setColour(color c)
{
    global_color = c;
}
char getColor()
{
    return global_color;
}
void puts_c(const char *str, color c)
{
    color old = global_color;
    global_color = c;
    for (const char *p = str; *p != 0; p++)
        putchar(*p);
    global_color = old;
}
void putchar_c(const char ch, color C)
{
#ifdef RV32
    color old = global_color;
    global_color = C;
    if (ch == BACKSPACE) // backspace
    {
        vga_ch--;
        vga_start[(vga_ch << 5) + vga_line] = 0;
        if (vga_ch == 0)
        {
            if (vga_line)
            {
                vga_line--;
            }
            else
            {
                vga_line = 31;
            }
            vga_ch = ends[vga_line];
        }
        (*vga_cursor_p) = cursor_loc;
        return;
    }
    if (ch == 10) // enter
    {
        ends[vga_line] = vga_ch;
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
        (*vga_cursor_p) = cursor_loc;
        return;
    }

    if (vga_ch == VGA_MAXCOL)
    {
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
    }
    vga_start[(vga_ch << 5) + vga_line] = ch;
    color_start[(vga_ch << 5) + vga_line] = global_color;
    vga_ch++;
    if (vga_ch >= VGA_MAXCOL)
    {
        ends[vga_line] = VGA_MAXCOL;
        vga_ch = 1;
        vga_line++;
        vga_rollLine();
    }
    (*vga_cursor_p) = cursor_loc;
    global_color = old;
    return;
#endif
}
void displayChar(int x,int y,char ch,color c)
{
    vga_start[(x << 5) + y] = ch;
    color_start[(x << 5) + y] = c;
}