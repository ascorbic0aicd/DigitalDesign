#define VGA_START 0x00200000
#define KEY_LOC 0x00300000
#define VGA_MAXLINE 30
#define VGA_MAXCOL 70
#define RV32
#ifdef RV32
#define NULL 0xFFFFFFFF
void puts(char* str);
void putchar(char ch);
void vga_init(void);
char getchar();
void gets(char *str);
unsigned int __mulsi3(unsigned int a, unsigned int b);
unsigned int __umodsi3(unsigned int a, unsigned int b);
unsigned int __udivsi3(unsigned int a, unsigned int b);
#else
#include<stdio.h>
#endif

