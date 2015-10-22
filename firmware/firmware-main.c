#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"

extern char __rodata_end__[];
extern char __data_begin__[],__data_end__[];
extern char __bss_begin__[],__bss_end__[];

#define PA_DISK 0x1f000020
#define PA_PRINTER 0x1f000010
#define BLOCK_SIZE 512
#define BLOCK_MASK 0x1ff

void reloc_data_bss(void){
   memcpy(__data_begin__, __rodata_end__, __data_end__ - __data_begin__);
   memset(__bss_begin__, 0, __bss_end__ - __bss_begin__);
}

void kputs(const char *buf);
int kprintf(char *fmt,...);
void readdisk(size_t sector_num, ssize_t offset, void *buf, size_t len);
void writedisk(size_t sector_num, ssize_t offset, void *buf, size_t len);



void _read_block_(size_t sector_num, void *buf, int nonblock){
   *(size_t *)(PA_DISK+0xa0000000) = (size_t *)(buf-0x80000000);///PA
   *(size_t *)(PA_DISK+0xa0000004) = sector_num;
   *(size_t *)(PA_DISK+0xa0000008) = 0x01;
   while(!nonblock && (*(size_t *)(PA_DISK+0xa0000008)|0x3)==0x3);///wait for pending
}
void read_block(size_t sector_num, void *buf){
   _read_block_(sector_num,buf,0);
}
void _write_block_(size_t sector_num, void *buf, int nonblock){
   *(size_t *)(PA_DISK+0xa0000000) = (size_t *)(buf-0x80000000);///PA
   *(size_t *)(PA_DISK+0xa0000004) = sector_num;
   *(size_t *)(PA_DISK+0xa0000008) = 0x02;
   while(!nonblock && (*(size_t *)(PA_DISK+0xa0000008)|0x3)==0x3);///wait for pending
}
void write_block(size_t sector_num, void *buf){
   _write_block_(sector_num,buf,0);
}

unsigned char DISK_BUFF[BLOCK_SIZE];
void readdisk(size_t sector_num, ssize_t offset, void *buf, size_t len){
   unsigned char *addr;
   addr = buf;
   read_block(sector_num++,DISK_BUFF);
   while(len>0){
      if(offset>=BLOCK_SIZE){//mem miss
         read_block(sector_num++,DISK_BUFF);
         offset-=BLOCK_SIZE;
      }
      *addr=DISK_BUFF[offset++];
      ++addr;
      --len;
   }
}

void writedisk(size_t sector_num, ssize_t offset, void *buf, size_t len){
   unsigned char *addr;
   ;
}

size_t disk_size(){
   volatile unsigned int size,DMA_addr,sector,status,i;
   DMA_addr = *(size_t *)(PA_DISK+0xa0000000);
   sector = *(size_t *)(PA_DISK+0xa0000004);
   status = *(size_t *)(PA_DISK+0xa0000008);
   size =  *(size_t *)(PA_DISK+0xa000000c);
   kprintf("DMA:%x\nSector:%x\nStatus:%x\nSize:%x\n",DMA_addr,sector,status,size);
   return size;
}


int g = 6;

void change_global_data(void){
   kprintf("g = %d\n",g);
   kprintf("addr g = %x\n",&g);
   g = 8;
   kprintf("g = %d\n",g);
}

int main(void){
    unsigned char buff[1100];
    int a;
   //while(1)
    reloc_data_bss();
    kputs("Hello there!\n");

    change_global_data();

    kprintf("Disk size: %x\n",disk_size());
    _read_block_(0,buff,1);
    kprintf("state:%x\n",*(unsigned int *)(PA_DISK+0xa0000008));
    kprintf("state:%x\n",*(unsigned int *)(PA_DISK+0xa0000008));
    kprintf("Data:%x\n",buff[0]);
    buff[0]=buff[0]+1; 
    buff[1]=buff[0]+0x10;
    _write_block_(0,buff,1);
    kprintf("state:%x\n",*(unsigned int *)(PA_DISK+0xa0000008));
    kprintf("state:%x\n",*(unsigned int *)(PA_DISK+0xa0000008));
    buff[0]=0xf;
    buff[1]=0xf;
    buff[2]=0xf;
    buff[3]=0xf;
    //read_block(0,buff);
    readdisk(0,1,buff,2);
    kprintf("state:%x\n",*(unsigned int *)(PA_DISK+0xa0000008));
    kprintf("state:%x\n",*(unsigned int *)(PA_DISK+0xa0000008));
    for(a=0;a<5;++a)
       kprintf("%x ",buff[a]);
    kprintf("Data:%x\n",buff[0]);
    

    while(1);/* endless loop */
    return 0;
}

int kprintf(char *fmt,...){
   int res;
   char buffer[BUFSIZ];
   va_list ap;
   va_start(ap,fmt);
   res = vsnprintf(buffer,BUFSIZ,fmt,ap);
   kputs(buffer);
   va_end(ap);  
   return res;
}

void kputs(const char *buf){
    // Printer: 0x1f000010 | 0xbf000010 
    volatile unsigned char *pointer = (volatile unsigned char *)(PA_PRINTER+0xa0000000);
    for(; *buf != '\0';++buf)
        *pointer = *buf;

}

