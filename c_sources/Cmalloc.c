//#include "sys.h"
#include "Cmalloc.h"
#include "Qstring.h"

#ifdef RV32
#else
char HEAP_ADDR[0x100000];
#endif

int has_initialized = 0;  //初始化标志

//堆空间初始化
void Cmalloc_init(void)
{
    mem_control_block * mcb = NULL;
    mcb = (mem_control_block *)HEAP_START;
    mcb->is_available = 1;
    mcb->prior_blocksize = 0;
    mcb->current_blocksize = HEAP_SIZE - sizeof(mem_control_block);

    has_initialized = 1;
}

void * Cmalloc(unsigned int numbytes)
{
    unsigned int current_location, otherbck_location;
    mem_control_block * current_location_mcb = NULL,* other_location_mcb = NULL;
    void * memory_location = NULL;  //可用的block数据区首地址
    unsigned int process_blocksize;

    if(has_initialized == 0) {
        Cmalloc_init();
    }

    current_location = (unsigned)HEAP_START;
    //寻找可用的堆空间
    while(current_location < (unsigned)HEAP_END){  
        current_location_mcb = (mem_control_block *)current_location;
        if(current_location_mcb->is_available){
            //block空间大小正好
            if((current_location_mcb->current_blocksize == numbytes)){   
                current_location_mcb->is_available = 0; 
                memory_location = (void *)(current_location + sizeof(mem_control_block));
                break;
            }
            //block空间太大(大于控制区域和所需数据区域总和)
            else if(current_location_mcb->current_blocksize >= numbytes + sizeof(mem_control_block)){
                current_location_mcb->is_available = 0;
                process_blocksize = current_location_mcb->current_blocksize;
                current_location_mcb->current_blocksize = numbytes;
                //将这大块分割
                other_location_mcb = (mem_control_block *)(current_location + numbytes + sizeof(mem_control_block));
                other_location_mcb->is_available = 1;
                other_location_mcb->prior_blocksize = numbytes;
                other_location_mcb->current_blocksize = process_blocksize - numbytes - sizeof(mem_control_block);
                otherbck_location = current_location + process_blocksize + sizeof(mem_control_block); 
    
                if(otherbck_location < (unsigned)HEAP_END){
                    other_location_mcb = (mem_control_block *)(otherbck_location);
                    other_location_mcb->prior_blocksize = process_blocksize - numbytes - sizeof(mem_control_block);
                }
                
                memory_location = (void *)(current_location + sizeof(mem_control_block));
                break;
            } 
        }
        //遍历下一块block(数据区+控制区)
        current_location += current_location_mcb->current_blocksize + sizeof(mem_control_block);
    }

    if(memory_location == NULL) {
        return NULL;     //堆满
    }
    return memory_location; 
}

void Cfree(void *firstbyte)
{
    unsigned int current_location,otherbck_location;
    mem_control_block * current_mcb = NULL,* next_mcb = NULL,* prior_mcb = NULL,* other_mcb = NULL;
    current_location = (unsigned int)firstbyte - sizeof(mem_control_block);
    current_mcb = (mem_control_block *)current_location;
    current_mcb->is_available = 1;

    otherbck_location = current_location + sizeof(mem_control_block) + current_mcb->current_blocksize;
    if(otherbck_location < (unsigned)HEAP_END){
        //判断后一块是否可整合
        next_mcb = (mem_control_block *)otherbck_location;
        //整合可用空间
        if(next_mcb->is_available > 0){
            current_mcb->current_blocksize = current_mcb->current_blocksize + sizeof(mem_control_block) + next_mcb->current_blocksize;
            otherbck_location = current_location + sizeof(mem_control_block) + current_mcb->current_blocksize;
            if(otherbck_location < (unsigned)HEAP_END){
                other_mcb = (mem_control_block *)otherbck_location;
                other_mcb->prior_blocksize = current_mcb->current_blocksize;
            }
        }
    }

    if(current_location > (unsigned)HEAP_START){
        //判断前一块是否可整合
        prior_mcb = (mem_control_block *)(current_location - sizeof(mem_control_block) - current_mcb->prior_blocksize);
        //整合可用空间
        if ((unsigned)prior_mcb >= (unsigned)HEAP_START && prior_mcb->is_available > 0)
        {
            prior_mcb->current_blocksize = prior_mcb->current_blocksize + sizeof(mem_control_block) + current_mcb->current_blocksize;
            otherbck_location = current_location + sizeof(mem_control_block) + current_mcb->current_blocksize;
            if(otherbck_location < (unsigned)HEAP_END){
                other_mcb = (mem_control_block *)otherbck_location;
                other_mcb->prior_blocksize = prior_mcb->current_blocksize;
            }
        }
    }
}

void Cmprint(void)
{
    unsigned int current_location = HEAP_START;
    mem_control_block * current_location_mcb = NULL;
    
    //寻找可用的堆空间
    unsigned k=1;
    char str[10];

    puts("sizeof(mem_control_block) is");
    Qitos(str, sizeof(mem_control_block));
    puts(str);
    puts("  ");
    while(current_location < (unsigned)HEAP_END){  
        current_location_mcb = (mem_control_block *)current_location;
        puts("-----There is the");
        Qitos(str,k);
        puts(str);
        //puts("block");

        //puts("its is_available is");
        if (current_location_mcb->is_available > 0){puts("avilable");}
        else{puts("full");}
            

        //puts("its datasize is");
        Qitos(str,current_location_mcb->current_blocksize);
        puts(str);
        //遍历下一块block(数据区+控制区)
        current_location += current_location_mcb->current_blocksize + sizeof(mem_control_block);
        k++;
    }
    puts("  ");
}
int Cmnum(void){
    unsigned int current_location = (unsigned)HEAP_START;
    mem_control_block *current_location_mcb = NULL;

    // 寻找可用的堆空间
    unsigned k = 0;
    
    while (current_location < (unsigned)HEAP_END)
    {
        current_location_mcb = (mem_control_block *)current_location;
        if (current_location_mcb->is_available>0)
        {
            ;
        }
        else
        {
            k++;
        }

        
        // 遍历下一块block(数据区+控制区)
        current_location += current_location_mcb->current_blocksize + sizeof(mem_control_block);
      
    }
    return k;
}