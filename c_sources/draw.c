#include "sys.h"

char data1[] = {"    _____                         ____                 "};
char data2[] = {"   / ____|                       / __ \\                "};
char data3[] = {"  | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ "};
char data4[] = {"  | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|"};
char data5[] = {"  | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   "};
char data6[] = {"   \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   "};
 
 
#define OFFSET_Y 9
#define OFFSET_X 6
void drawGameOver()
{
    vga_init();
    for (int i = 0; data1[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 0, data1[i], WHITE);
    }
    for (int i = 0; data2[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 1, data2[i], WHITE);
    }

    for (int i = 0; data3[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 2, data3[i], WHITE);
    }
    for (int i = 0; data4[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 3, data4[i], WHITE);
    }

    for (int i = 0; data5[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 4, data5[i], WHITE);
    }
    for (int i = 0; data6[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 5, data6[i], WHITE);
    }
}