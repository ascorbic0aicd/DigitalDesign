#include "sys.h"

char data1[] = {"    _____                         ____                 "};
char data2[] = {"   / ____|                       / __ \\                "};
char data3[] = {"  | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ "};
char data4[] = {"  | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|"};
char data5[] = {"  | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   "};
char data6[] = {"   \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   "};
 
                                                       
char LOGO[][80] = { "                        ,l                     ",         
                    "                        xX.                       ",      
                    "     .;                .NMl                .,     ",      
                    "      ck.           ..'lMM0;'.            :O.     ",      
                    "       dNc     .cxKWMWKNMMMXNWWXOo,     .kX.      ",      
                    "        dM0'.lKMMW0o;..0MMM:.'cxNMMWk, oWX'.      ",      
                    "         lWWWMMNd'...  XMMMc.   .:OMMMWM0'.       ",      
                    "         ,XMMMM0o,     NMMMl.    ;xNMMMMx.        ",      
                    "        ;WMMWdkWMMXo. .WMMMo. ;OWMMKo0MMM0.       ",      
                    ".      .WMMMo...lXMMN,.NMMMo.OMMWO;...NMMMk      .",      
                    ".oOd:. dMMMW,.   .cKMX.KMMM:oMWx,..   xMMMW..,lkk,",      
                    "  .OMMNNMMMW.       ;0OxMMXlXo'..     dMMMMNWMXc. ",      
                    "    ;XMMMMMM: .cl,    ;0MMNd..  .cl,  0MMMMMWx..  ",      
                    "     .dWMMMM0.  'kNOc. ,MMx..:xXXc.  :MMMMMK;..   ",      
                    "       'OMMMMk.   'kWMNKMMXXWMKl..  ;WMMMXo..     ",      
                    "         ,OWMMK,    .dNMMMMM0c..  .dWMMXo..       ",      
                    "         . 'xNMMO;    .oMM0;..  .oNMW0c...        ",      
                    "         .ll,.;dKWNOo;''WMd,;lkXMXOl,,ld;         ",      
                    "           ,0W0dc;:lxOKXMMNK0xoc::oOXNo.          ",      
                    "             ;OWMMWX0kxdWMOdxOKNMMMNd..           ",      
                    "               'xNMMMMMNWMNMMMMMW0l...            ",      
                    "                 .':ol;'NMl'col;'..               ",      
                    "                    ;NNkWMOKWk.                   ",      
                    "                     ;WMMMMM0.                    ",      
                    "                      :WMMM0'.                    ",      
                    "                       :WMk'.                     ",      
                    "                        ':..  ",
                    "[                                                 ]"};
#define OFFSET_Y 9
#define OFFSET_X 6
void drawGameOver()
{
    vga_init();
    //setColour(YELLOW);
    for (int i = 0; data1[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 0, data1[i], YELLOW);
    }
    for (int i = 0; data2[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 1, data2[i], YELLOW);
    }

    for (int i = 0; data3[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 2, data3[i], YELLOW);
    }
    for (int i = 0; data4[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 3, data4[i], YELLOW);
    }

    for (int i = 0; data5[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 4, data5[i], YELLOW);
    }
    for (int i = 0; data6[i] != 0; i++)
    {
        displayChar(i + OFFSET_X, OFFSET_Y + 5, data6[i], YELLOW);
    }
}
void drawLOGO()
{
    enCursor(false);
    vga_init();
      
    for (int i = 0; i < 27; i++)
    {

        for (int J = 0; LOGO[i][J] != 0 ; J++)
        {
            displayChar(J + 9,i,LOGO[i][J],RED);
        }
        
    }
    for (int J = 0; LOGO[27][J] != 0; J++)
    {
        displayChar(J + 9, 27, LOGO[27][J], WHITE);
    }
    //sleep(2000);
    for (int i = 1; i <= 49; i++)
    {
        displayChar(i + 9, 27, '=', WHITE);
        sleep(100);

    }
    enCursor(true);
}