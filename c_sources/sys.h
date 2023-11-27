#define VGA_START 0x00200000
#define VGA_MAXLINE 30
#define VGA_MAXCOL 70
void putstr(char* str);
void putch(char ch);
void vga_init(void);