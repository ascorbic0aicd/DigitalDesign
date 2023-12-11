
//#include <stdlib.h>
//#include <conio.h> // Windows-specific，用于读取键盘输入
//#include <windows.h>

#include "Qrand.h"
#include "Qstring.h"
#include "sys.h"

#define Height 28
#define Width 40
#define Py_x 10
#define Py_y 1
// 定义一个 10行 15列 的游戏区域
int game_area[Height][Width] = { 0 };
int color_area[Height][Width] = { 0 };
// 定义方块形状
int block[7][4] = {
  {1, 3, 5, 7}, // I
  {2, 4, 5, 7}, // Z
  {3, 5, 4, 6}, // S
  {3, 5, 4, 7}, // T
  {2, 3, 5, 7}, // L
  {3, 5, 7, 6}, // J
  {2, 3, 4, 5}  // O
};

// 定义游戏参数
int score = 0;  // 当前得分
int level = 1;  // 当前等级
int speed = 300; // 游戏速度（毫秒数）
int line_count = 0; // 当前消除的行数

// 定义方块的位置和类型
int block_type;
int block_x_pos, block_y_pos;
int block_rotation;
int block_color;

// 用于判断落地的函数
int can_move(int dx, int dy, int rotation) {
    int x, y, new_x, new_y;
    for (int i = 0; i < 4; i++) {
        x = block[block_type][i] % 2;  
        y = block[block_type][i] / 2;
        // 确定新的位置和方向
        switch (rotation % 4) {
        case 0: new_x = x; new_y = y; break;
        case 1: new_x = -y; new_y = x; break;
        case 2: new_x = -x; new_y = -y; break;
        case 3: new_x = y; new_y = -x; break;
        }
        // 如果当前位置无法到达新的位置，返回False
        if (new_x + block_x_pos + dx < 0 || new_x + block_x_pos + dx >= Width || new_y + block_y_pos + dy < 0 || new_y + block_y_pos + dy >= Height) {
            return 0;
        }
        if (game_area[new_y + block_y_pos + dy][new_x + block_x_pos + dx]) {
            return 0;
        }
    }
    return 1;
}

// 方块移动函数
void move(int dx, int dy, int rotation) {
    if (can_move(dx, dy, rotation)) {
        block_x_pos += dx;
        block_y_pos += dy;
        block_rotation = rotation;
    }
}

// 消除行函数
void clear_lines() {
    int full;
    int lines_cleared = 0;
    for (int i = 0; i < Height; i++) {
        full = 1;
        for (int j = 0; j < Width; j++) {
            if (game_area[i][j] == 0) {
                full = 0;
                break;
            }
        }
        if (full) {
            // 清除该行
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < Width; j++) {
                    game_area[k][j] = game_area[k - 1][j];
                    color_area[k][j] = color_area[k-1][j];
                }
            }
            lines_cleared++;
        }
    }
    // 根据消除的行数增加得分
    switch (lines_cleared) {
    case 1: score += 10; break;
    case 2: score += 30; break;
    case 3: score += 60; break;
    case 4: score += 100; break;
    }
    line_count += lines_cleared;
    // 根据消除的行数提升等级
    level = line_count / 10 + 1;
    // 根据等级调整速度
    speed = 400 - (level - 1) * 50;
}

// 生成新的方块函数
void spawn() {
    block_type = Qrand() % 7;
    block_x_pos = 6;
    block_y_pos = 0;
    block_rotation = 0;
    block_color = Qrand()%8;
    while(block_color == BLACK){block_color = Qrand()%8;}
}

// 更新游戏区域函数
void update_game_area() {
    // 将当前方块放置到游戏区域
    int x, y, new_x, new_y;
    for (int i = 0; i < 4; i++) {
        x = block[block_type][i] % 2;
        y = block[block_type][i] / 2;
        switch (block_rotation % 4) {
        case 0: new_x = x; new_y = y; break;
        case 1: new_x = -y; new_y = x; break;
        case 2: new_x = -x; new_y = -y; break;
        case 3: new_x = y; new_y = -x; break;
        }
        game_area[new_y + block_y_pos][new_x + block_x_pos] = 1;
        color_area[new_y + block_y_pos][new_x + block_x_pos] = block_color;
    }
}

// 输出游戏界面
void print_game_area() {
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (game_area[i][j]) {
                displayChar(j + Py_x, i + Py_y, 'O', color_area[i][j]);
                //printf("■ ");
            }
            else {
                displayChar(j + Py_x, i + Py_y, ' ', WHITE);
                //printf("  ");
            }
        }
        //printf("\n");
    }
    for(int i = -1; i <= Height; i++){
        displayChar(Py_x - 1, i + Py_y, 'X', WHITE);
        displayChar(Py_x  + Width, i + Py_y, 'X', WHITE);
    }
    for(int j = -1; j <= Width; j++){
        displayChar(j + Py_x, Py_y - 1, 'X', WHITE);
        displayChar(j + Py_x, Height + Py_y, 'X', WHITE);
    }

    char sco_s = score / 10 + '0';
    if(sco_s == '0')sco_s=' ';
    char sco_g = score % 10 + '0';
    displayChar(1 + Width + Py_x + 5, 10, 'S', WHITE);
    displayChar(2 + Width + Py_x + 5, 10, 'c', WHITE);
    displayChar(3 + Width + Py_x + 5, 10, 'o', WHITE);
    displayChar(4 + Width + Py_x + 5, 10, 'r', WHITE);
    displayChar(5 + Width + Py_x + 5, 10, 'e', WHITE);
    displayChar(6 + Width + Py_x + 5, 10, ':', WHITE);
    displayChar(7 + Width + Py_x + 5, 10, sco_s, WHITE);
    displayChar(8 + Width + Py_x + 5, 10, sco_g, WHITE);

    char levl_s = level / 10 + '0';
    if(levl_s == '0')levl_s=' ';
    char levl_g = level % 10 + '0';
    displayChar(1 + Width + Py_x + 5, 13, 'L', WHITE);
    displayChar(2 + Width + Py_x + 5, 13, 'e', WHITE);
    displayChar(3 + Width + Py_x + 5, 13, 'v', WHITE);
    displayChar(4 + Width + Py_x + 5, 13, 'e', WHITE);
    displayChar(5 + Width + Py_x + 5, 13, 'l', WHITE);
    displayChar(6 + Width + Py_x + 5, 13, ':', WHITE);
    displayChar(7 + Width + Py_x + 5, 13, levl_s, WHITE);
    displayChar(8 + Width + Py_x + 5, 13, levl_g, WHITE);
    //printf("Score: %d\n", score);
    //printf("Level: %d\n", level);
}

void tetris_exec() {
    // 初始化游戏区域
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            game_area[i][j] = 0;
            color_area[i][j] = 0;
        }
    }
    // 生成新的方块
    Qsrand(0);
    spawn();
    while (1) {
        // 更新游戏区域
        update_game_area();
        // 显示游戏界面
        print_game_area();
        // 延迟一段时间，实现动态效果
        sleep(speed);
        // 清除当前方块
        int x, y, new_x, new_y;
        for (int i = 0; i < 4; i++) {
            x = block[block_type][i] % 2;
            y = block[block_type][i] / 2;
            switch (block_rotation % 4) {
            case 0: new_x = x; new_y = y; break;
            case 1: new_x = -y; new_y = x; break;
            case 2: new_x = -x; new_y = -y; break;
            case 3: new_x = y; new_y = -x; break;
            }
            game_area[new_y + block_y_pos][new_x + block_x_pos] = 0;
            color_area[new_y + block_y_pos][new_x + block_x_pos] = 0;
        }
        // 检查是否可以向下移动方块
        if (can_move(0, 1, block_rotation)) {
            //block_y_pos++;
            move(0, 1, block_rotation);
        }
        else {
            // 方块无法继续向下移动，生成新的方块并检查是否有可消除的行
            update_game_area();
            clear_lines();
            spawn();
            // 检查游戏是否结束（即游戏区域顶部有方块）
            int game_over = 0;
            for (int i = 0; i < Width; i++) {
                if (game_area[0][i] == 1) {
                    game_over = 1;
                    break;
                }
            }
            if (game_over) {
                break;
            }
        }
        // 处理用户输入
        if (kbhit()) {
            char input = getchar();
            switch (input) {
            case 'W': case 'w': move(0, 0, block_rotation + 1); break;
            case 'A': case 'a': move(-1, 0, block_rotation); break;
            case 'S': case 's': move(0, 1, block_rotation); break;
            case 'D': case 'd': move(1, 0, block_rotation); break;
            case 'Q': case 'q': move(0, 0, block_rotation - 1); break;
            default: break;
            }
        }
        
    }
    // 游戏结束，显示最终得分
    puts("Game Over!");
    //printf("Game Over\n");
    //printf("Final Score: %d\n", score);
    //return 0;
}

void tetris(int arg, char args[][50]){
    if (arg > 1)
    {
        puts("There are to many args!");
    }
    else
    {
        vga_init();
        //snake_init();
        tetris_exec();
        vga_init();
    }
}