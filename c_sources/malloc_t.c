//#include "sys.h"
#include "Cmalloc.h"
#include "QString.h"

#ifdef RV32

#else

void malloc_t(int arg, char args[][50])
{
    srand(time(0));
    int ff=1;
    char str[10];
    while(ff){
        ff=1;
        unsigned num = rand() % 10;
        char f[100];
        for(int i=0;i<100;i++)f[i]=0;
        int* arrp[100];
        for(int i=0;i<num;i++){
            unsigned csize=rand()%80;
            arrp[i]=(int*)Cmalloc(csize);
            f[i]=1;
        }
        puts("malloc new:");
        Cmprint();

        for(int i=0;i<num;i++){
            unsigned dnum;
            dnum = rand() % num;
            while (f[dnum] == 0)
            {
                dnum = rand() % num;
            }

            if(f[dnum]){
                Cfree(*(arrp+dnum));
                f[dnum]=0;
                Qitos(str, dnum);
                puts(str);
                if(i==num / 2){
                
                    puts("false");

                    puts("the num of args");
                    Qitos(str, num);
                    puts(str);

                    puts("the num of avai");
                    Qitos(str, Cmnum());
                    puts(str);

                    puts("the num of should");
                    Qitos(str, num - i - 1);
                    puts(str);

                    puts("the location");
                    Qitos(str, *(arrp + dnum));
                    puts(str);

                    puts("the start location");
                    Qitos(str, *arrp);
                    puts(str);

                    puts("the heap loc");
                    Qitos(str, HEAP_START);
                    puts(str);

                    Qitos(str,i);
                    puts(str);
                    Qitos(str, dnum);
                    puts(str);

                    puts("free:");
                    Cmprint();
                    ff=0;
                    break;
                
                }
            }
        }

    }

    // int *arrp[100];
    // for(int i=1;i<arg;i++){
    //     unsigned n = Qstoi(args[i]);
    //     arrp[i]=(int*)Cmalloc(n);
    //     puts("malloc new:");
    //     Cmprint();
        
    // }
    // for(int i=1;i<arg;i++){
    //     Cfree((int *)arrp + i);
    //     puts("free:");
    //     Cmprint();
    //     if (Cmnum() != arg - i)
    //     {
    //         puts("false");
    //         char str[10];
    //         Qitos(str, i);
    //         puts(str);
    //         break;
    //     }
    // }

}
#endif