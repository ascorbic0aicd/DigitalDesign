
#include "sys.h"
#include "Qrand.h"
#include "Qstring.h"

#define Height 20
#define Width 69
#define Py_h 5
#define Py_w 1
#define bar_num 3

int bird_x, bird_y;                // 小鸟的坐标 
int bar1_y[3], bar1_xDown[3], bar1_xTop[3]; // 障碍物1的相关坐标
int bird_score;                         // 得分，经过障碍物的个数

void startup() // 数据初始化
{
    
    bird_x = Height / 2;  //x在height方向，y在width方向
    bird_y = Width / 4 - 2;
    bar1_y[0] = Width / 2;
    bar1_y[1] = Width / 4 * 3;
    bar1_y[2] = Width;
    bar1_xDown[0] = Height / 3;
    bar1_xDown[1] = Height / 3;
    bar1_xDown[2] = Height / 3;
    bar1_xTop[0] = Height / 2;
    bar1_xTop[1] = Height / 2;
    bar1_xTop[2] = Height / 2;
    bird_score = 0;
}

void show() // 显示画面
{
    // for(int i = -1; i <= Height; i++){
    //     displayChar(Py_w - 1, i + Py_h, 'X', GREEN);
    //     displayChar(Py_w  + Width, i + Py_h, 'X', GREEN);
    // }
    for(int j = 0; j <= Width; j++){
        displayChar(j + Py_w, Py_h - 1, 'X', BLUE);
        displayChar(j + Py_w, Height + Py_h, 'X', BLUE);
    }
    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            // if ((i == bird_x) && (j == bird_y)){
            //     displayChar(j + Py_w, i + Py_h, 'O', YELLOW);
            //     displayChar(j + Py_w + 1, i + Py_h, '>', YELLOW);
            //     //printf(">"); //   输出小鸟
            // }
            if ((j == bar1_y[0]) && ((i < bar1_xDown[0]) || (i > bar1_xTop[0]))){
                displayChar(j + Py_w, i + Py_h, 'X', GREEN);
            }
            else if ((j == bar1_y[1]) && ((i < bar1_xDown[1]) || (i > bar1_xTop[1]))){
                displayChar(j + Py_w, i + Py_h, 'X', GREEN);
            }
            else if ((j == bar1_y[2]) && ((i < bar1_xDown[2]) || (i > bar1_xTop[2]))){
                displayChar(j + Py_w, i + Py_h, 'X', GREEN);
            }
            else{
                displayChar(j + Py_w, i + Py_h, ' ', GREEN);
            }
        }
    }
    displayChar(bird_y + Py_w, bird_x + Py_h, '>', YELLOW);
    displayChar(bird_y + Py_w - 1, bird_x + Py_h, 'O', YELLOW);

    char sco_s = bird_score / 10 + '0';
    if(sco_s == '0')sco_s=' ';
    char sco_g = bird_score % 10 + '0';
    displayChar(1 + Py_w, Py_h - 2, 'S', WHITE);
    displayChar(2 + Py_w, Py_h - 2, 'c', WHITE);
    displayChar(3 + Py_w, Py_h - 2, 'o', WHITE);
    displayChar(4 + Py_w, Py_h - 2, 'r', WHITE);
    displayChar(5 + Py_w, Py_h - 2, 'e', WHITE);
    displayChar(6 + Py_w, Py_h - 2, ':', WHITE);
    displayChar(7 + Py_w, Py_h - 2, sco_s, WHITE);
    displayChar(8 + Py_w, Py_h - 2, sco_g, WHITE);

    //printf("得分：%d\n", score);
}

void bird_exec()
{
    startup(); 
    Qsrand(time(0));
    show(); 
    int game_over = 1 ;
    while (1)  
    {         
        bird_x++;
        for(int i=0;i<bar_num;i++){
            bar1_y[i]--;
        }

        char input;
        char fget = kbhit(150,0);
        if(fget != 0){
            input = fget;
            if (input == ' ')
                bird_x = bird_x - 3;
        }
        if(bird_x < 0 || bird_x >= Height){
            break;
        }

        if (bird_y == bar1_y[0])
        {
            if ((bird_x >= bar1_xDown[0]) && (bird_x <= bar1_xTop[0]))
                bird_score++;
            else
            {
                break;
            }
        }
        if (bar1_y[0] <= bird_y - 5) // 再新生成一个障碍物
        {
            for(int i=0;i< bar_num - 1;i++){
                bar1_y[i]=bar1_y[i+1];
                bar1_xDown[i]=bar1_xDown[i+1];
                bar1_xTop[i]=bar1_xTop[i+1];
            }
            bar1_y[bar_num -1 ] = Width;
            int temp = Qrand() % (int)(Height * 0.7);
            bar1_xDown[bar_num -1] = temp - Height / 10;
            bar1_xTop[bar_num -1] = temp + Height / 10;
        }
        show();  

    }
    puts("Game Over!");
    return ;
}

void bird(int arg, char args[][50]){
    if (arg > 1)
    {
        puts("There are to many args!");
    }
    else
    {
        vga_init();
        bird_exec();
        vga_init();
    }
}