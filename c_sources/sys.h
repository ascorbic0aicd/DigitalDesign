#define VGA_START 0x00200000
#define KEY_LOC 0x00300000
#define CHECK_KEY 0x10300000
#define VGA_LINE_NO 0x00400000
#define COLOR_REG 0x00500000
#define CURSOR_ADDR 0x00600000
#define TIMER_ADDR 0x00700000
#define VGA_MAXLINE 30
#define VGA_MAXREG 31
#define VGA_MAXCOL 70
#define BACKSPACE 8
typedef enum
{
    WHITE = 0,
    RED = 1,
    BLUE = 2,
    GREEN = 3,
    BLACK = 4,
    YELLOW = 5,
    ORANGE = 6,
    PURPLE = 7
} color;
#include "Qrand.h"
void setColour(color c);
char getColor();
void vga_init(void);
void putchar_c(const char ch, color C);
void puts_c(const char *str,color c);
typedef enum{true = 1,false = 0} bool;
#define RV32
#ifdef RV32
#define NULL (void *)0xFFFFFFFF
    extern int vga_line;
void puts(const char* str);
void putchar(const char ch);
char getchar();
void gets(char *str);
void vga_rollLine();
void vga_clear_line(int y); 
int time(int _);
bool kbhit();
void sleep(int d);
#define assert(expr) if(!expr) puts("something error!\n");
#else
#include<stdio.h>
#include<time.h>
#endif

