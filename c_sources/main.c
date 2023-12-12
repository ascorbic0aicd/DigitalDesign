#include "sys.h"
#include "Qash.h"

int main();

//setup the entry point
#ifdef RV32
void entry()
{
    asm("lui sp, 0x00120"); //set stack to high address of the dmem
    asm("addi sp, sp, -4");
    main();
}
#endif
int main()
{
#ifdef RV32
    vga_init();
    drawLOGO();
    //sleep(3000);
    vga_init();
#endif
    Qash();
    return 0;
}
