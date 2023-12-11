
#include "Qrand.h"
#include "Cmalloc.h"
#include "Qstring.h"
 
//声明储存蛇坐标的链表
typedef struct Snake{
    int position[2];
    struct Snake * next;
}sna;
#define Height 30
#define Width 40
//const int Width = 65;
//const int Height = 25;
//游戏地图
char Game[Height][Width];
//记录玩家的默认坐标

//玩家的上一个坐标
int BeforePosition[2];
//食物坐标
int FoodPosition[2];
 
//蛇头的头节点
struct Snake *Headf = NULL;
//蛇的长度，默认为1
int SnakeLength=1;
//分数
int Score = 0;
//游戏是否结束的变量
int IsOver = 0;
//方向
char Direction='#';
//随机生成食物
void Food(int pos[2]){
    
    FoodPosition[1] = Qrand() % (Width - 2) + 1;
    FoodPosition[0] = Qrand() % (Height - 2) + 1;
    while((FoodPosition[1] == pos[1] && FoodPosition[0] == pos[0])||Game[FoodPosition[0]][FoodPosition[1]]=='0'){
        FoodPosition[1] = Qrand() % (Width - 2) + 1;
        FoodPosition[0] = Qrand() % (Height - 2) + 1;
    }
}
void changeMap(struct Snake *Head)
{
    char str[20];
    struct Snake *p1 = Head;
    while (p1->next != NULL)
    {
        p1 = p1->next;
        SnakeLength += 1;
    }
    p1 = Head;

    for (int i = 0; i < Height; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            if (i == 0 || j == 0 || j == Width - 1 || i == Height - 1)
            {
                Game[i][j] = 'X';
            }
            else
            {
                Game[i][j] = ' ';
            }
        }
    }
    Game[FoodPosition[0]][FoodPosition[1]] = '@';

    for (int i = 0; i < SnakeLength; i++)
    {
        if (p1->position[0] < Height && p1->position[1] < Width && p1->position[0] > 0 && p1->position[1] > 0)
        {
            Game[p1->position[0]][p1->position[1]] = 'O';
            p1 = p1->next;
        }
    }
}
//创建蛇身体，通过链表进行链接每一块身体
struct Snake *CreatBody(struct Snake *Head,int position[2]){
    struct Snake *head;
    struct Snake *p1,*p2;
    head = Head;
    p1 = (struct Snake *)Cmalloc(sizeof(struct Snake));
    //puts("2");
    if (p1 != NULL) {
        p1->position[0] = position[0];
        p1->position[1] = position[1];
        p1->next = NULL;
    }
    
    if(head==NULL){
        head = p1;
    }else{
 
        p2 = head;
        while(p2->next!=NULL){
            p2 = p2->next;
        }
        p2->next = p1;
    }
    
    return head;
}
 
//绘出游戏界面
void DrawUI(struct Snake *Head){
    
    char str[20];
    struct Snake *p1=Head;
    while (p1->next!=NULL)
    { 
        p1 = p1->next;
        SnakeLength+=1;
    }
    p1 = Head;
    SnakeLength = 1 ;

    displayChar(51, 15, 'S', WHITE);
    displayChar(52, 15, 'c', WHITE);
    displayChar(53, 15, 'o', WHITE);
    displayChar(54, 15, 'r', WHITE);
    displayChar(55, 15, 'e', WHITE);
    displayChar(56, 15, ':', WHITE);
    char sc[20] = {0};
    Qitos(sc,Score);
    int len = Qstrlen(sc);
    for (int i = 0; i < len; i++)
    {
        displayChar(57 + i,15,sc[i],WHITE);
    }

    for (int i = 0; i < Height; i++){
        for(int j = 0; j < Width; j++){
            if(Game[i][j]=='O'){
                int color = Qrand()%8;
                while(color==4){color = Qrand()%8;}
                displayChar(j + 1, i, Game[i][j], color);
            }
            else if(Game[i][j]=='@'){
                displayChar(j + 1, i, Game[i][j], RED);
            }
            else if(Game[i][j]=='X'){
                displayChar(j + 1, i, Game[i][j], GREEN);
            }
            else{
                displayChar(j + 1, i, Game[i][j], WHITE);
            }
        }
    }
 
}
 
 
//蛇的移动逻辑
void Move(struct Snake *Head){
    struct Snake *p1,*p2;
    p1 = Head;p2 = Head;
    //方向
    //char Direction;
    int pos[2];
    int BeforePos[2];

    SnakeLength = 1;
    p1 = Head;
    while (p1->next!=NULL)
    { 
        p1 = p1->next;
        SnakeLength+=1;
    }
    p1 = Head;   
    // if(Game[BeforePos[0]][BeforePos[1]]=='X' || SnakeLength > 4 && Game[BeforePos[0]][BeforePos[1]]=='O'){
    //     IsOver = 1;
    //     return;
    // }
    

    

 
    if(Direction == 'w' || Direction == 'W'){
        if(Score == 0){
            p1->position[0] -= 1;
            Game[BeforePos[0]][BeforePos[1]]=' ';
        }else{
            p1 = Head;
            p2 = (struct Snake*)Cmalloc(sizeof(struct Snake));
            p2->position[0]=p1->position[0];
            p2->position[1]=p1->position[1];
            struct Snake *p3;
            p3 = p1->next;
            p1->next = p2;
            p2 ->next = p3;
            p1 = Head;
            while (p1->next != NULL)
            {
                p2 = p1;
                p1 = p1->next;                    
            }
            Game[p1->position[0]][p1->position[1]]=' ';
            BeforePosition[0]=p1->position[0];
            BeforePosition[1]=p1->position[1];
            p2->next = NULL;
            Cfree(p1);
            p1 = Head;                
            p1->position[0] -= 1;
        }  
    }
    if(Direction == 's' || Direction == 'S'){
        if(Score == 0){
                p1 = Head;
                p1->position[0] += 1;
                Game[BeforePos[0]][BeforePos[1]]=' ';
        }else{
            p1 = Head;
            p2 = (struct Snake*)Cmalloc(sizeof(struct Snake));
            p2->position[0]=p1->position[0];
            p2->position[1]=p1->position[1];
            struct Snake *p3;
            p3 = p1->next;
            p1->next = p2;
            p2 ->next = p3;
            p1 = Head;
            while (p1->next != NULL)
            {
                p2 = p1;
                p1 = p1->next;
            }
            BeforePosition[0]=p1->position[0];
            BeforePosition[1]=p1->position[1];
             Game[p1->position[0]][p1->position[1]]=' ';
            p2->next = NULL;
            Cfree(p1);
            p1 = Head;                
            p1->position[0] += 1;
        }
    }
    if(Direction == 'a' || Direction == 'A'){
        if(Score == 0){
            p1 = Head;
            p1->position[1] -= 1;
            Game[BeforePos[0]][BeforePos[1]]=' ';
        }else{
            p1 = Head;
            
            p2 = (struct Snake*)Cmalloc(sizeof(struct Snake));
            
            p2->position[0]=p1->position[0];
            p2->position[1]=p1->position[1];
            struct Snake *p3;
            p3 = p1->next;
            p1->next = p2;
            p2 ->next = p3;
            p1 = Head;
            while (p1->next != NULL)
            {
                p2 = p1;
                p1 = p1->next;
                // Game[p1->position[0]][p1->position[1]]=' ';
                
            }
            BeforePosition[0]=p1->position[0];
            BeforePosition[1]=p1->position[1];
             Game[p1->position[0]][p1->position[1]]=' ';
            p2->next = NULL;
            Cfree(p1);
            p1 = Head;                
            p1->position[1] -= 1;
        }
    }
    if(Direction == 'd'|| Direction == 'D'){
        if(Score == 0){
            p1 = Head;
            p1->position[1] += 1;
            Game[BeforePos[0]][BeforePos[1]]=' ';
        }else{
            p1 = Head;
            
            p2 = (struct Snake*)Cmalloc(sizeof(struct Snake));
            
            p2->position[0]=p1->position[0];
            p2->position[1]=p1->position[1];
            struct Snake *p3;
            p3 = p1->next;
            p1->next = p2;
            p2 ->next = p3;
            p1 = Head;
            while (p1->next != NULL)
            {
                p2 = p1;
                p1 = p1->next;
            }
            BeforePosition[0]=p1->position[0];
            BeforePosition[1]=p1->position[1];
            Game[p1->position[0]][p1->position[1]]=' ';
            p2->next = NULL;
            Cfree(p1);
            p1 = Head;               
            p1->position[1] += 1;
        }
    }
    if (SnakeLength > 4 && Game[p1->position[0]][p1->position[1]] == 'O')
    {
        IsOver = 1;
        return;
    }
    
    changeMap(Head);
    BeforePos[0] = p1->position[0];
    BeforePos[1] = p1->position[1];

    if (BeforePos[0] == 0 || BeforePos[0] == Height - 1 || BeforePos[1] == 0 || BeforePos[1] == Width - 1)
    {
        IsOver = 1;
        return;
    }
}
void snake_exec(){
    int PlayerPosition[2] = {Height / 2, Width / 2};
    Direction='#';
    Headf = NULL;
    SnakeLength=1;
    IsOver = 0;
    Score = 0;

    struct Snake *head;
    head = CreatBody(Headf, PlayerPosition);
    Food(head->position);
    Qsrand(0);
    IsOver = 0;
    DrawUI(head);

    while (!IsOver)
    {
        
        char fget = kbhit(125,Direction);

        if (fget != 0)
        {
            char dict = fget;
            if (Direction!=fget &&fget=='W'||fget == 'w'||fget == 'a'||fget == 'A'||fget == 's'||fget == 'S'||fget == 'D'||fget == 'd')
            {
                if ((dict == 'w' || dict == 'W') && (Direction == 's' || Direction == 'S') 
                || (dict == 's' || dict == 'S') && (Direction == 'w' || Direction == 'W') 
                || (dict == 'a' || dict == 'A') && (Direction == 'd' || Direction == 'D') 
                || (dict == 'd' || dict == 'D') && (Direction == 'a' || Direction == 'A'))
                {
                    Direction = Direction;
                }
                else
                {
                    Direction = fget;
                }
                
            }
        }
        clear_buf();
        struct Snake *p1 = head;
        Move(head);

        if (IsOver)
        {
            break;
        }
        if (p1->position[0] == FoodPosition[0] && p1->position[1] == FoodPosition[1])
        {
            Score += 1;
            head = CreatBody(head, BeforePosition);
            Food(head->position);
        }
        DrawUI(head);

    }
    
    puts("Game Over!");
    Cfree(head);
    head = NULL;
    Cfree(Headf);
    Headf = NULL;

}

void snake(int arg, char args[][50]){
    if (arg > 1)
    {
        puts("There are to many args!");
    }
    else
    {
        enCursor(false);
        vga_init();
        enCursor(true);
        snake_exec();
        sleep(500);
        vga_init();
    }
}