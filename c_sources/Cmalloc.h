#include "sys.h"

#ifdef RV32
#define HEAP_ADDR 0x00800000
#else
extern char HEAP_ADDR[0x100000];
#endif

#define HEAP_START (unsigned char *)HEAP_ADDR
#define HEAP_SIZE 0x100000
#define HEAP_END HEAP_START+HEAP_SIZE
//堆空间划分为一个个block，分为控制区域(mem_control_block)（在前）和数据区域(大小需要分配)（在后）
typedef struct{
unsigned char is_available; //1:空，可用  0:不可用
unsigned int prior_blocksize; 
unsigned int current_blocksize; //数据区域大小
}mem_control_block;

void Cmalloc_init(void);
void * Cmalloc(unsigned int numbytes);
void Cfree(void *firstbyte);
void Cmprint(void);
int Cmnum(void);
