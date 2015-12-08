#include "sys/types.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"
#include "config.h"

extern char __rodata_end__[];
extern char __data_begin__[],__data_end__[];
extern char __bss_begin__[],__bss_end__[];

void reloc_data_bss(void){
   memcpy(__data_begin__, __rodata_end__, __data_end__ - __data_begin__);
   memset(__bss_begin__, 0, __bss_end__ - __bss_begin__);
}

typedef void (*readdisk_t)(size_t,ssize_t,void *,size_t);
typedef int (*kprintf_t)(char *fmt,...);
//typedef void (*bootentry_t)(readdisk_t,kprintf_t);
typedef void (*bootentry_t)(readdisk_t,kprintf_t);




void kputs(const char *buf);
int kprintf(char *fmt,...);
void readdisk(size_t sector_num, ssize_t offset, void *buf, size_t len);
void writedisk(size_t sector_num, ssize_t offset, void *buf, size_t len);


void execute_mbr(void){
    unsigned char *mbr = (unsigned char *)MBR_INITIAL_ADDR;
    bootentry_t boot;
    //read_block(0,mbr);
    readdisk(0,0,mbr,SECTOR_SIZE);   
    boot = (bootentry_t)mbr; 
    (*boot)(readdisk,kprintf);
    //(*boot)(readdisk);
}


void _read_block_(size_t sector_num, void *buf, int nonblock){
   *(volatile size_t **)(PA_DISK+ADDR_PtoV2+DISK_DMA_OFFSET) = (size_t *)(buf-ADDR_PtoV1);///PA
   *(volatile size_t *)(PA_DISK+ADDR_PtoV2+DISK_SECTOR_OFFSET) = sector_num;
   *(volatile size_t *)(PA_DISK+ADDR_PtoV2+DISK_STATUS_OFFSET) = DISK_R_BIT;
   while(!nonblock && (*(size_t *)(PA_DISK+ADDR_PtoV2+DISK_STATUS_OFFSET)|DISK_RW_MASK)==DISK_RW_MASK);///wait for pending
}
void read_block(size_t sector_num, void *buf){
   _read_block_(sector_num,buf,0);
}
void _write_block_(size_t sector_num, void *buf, int nonblock){
   *(volatile size_t **)(PA_DISK+ADDR_PtoV2+DISK_DMA_OFFSET) = (size_t *)(buf-ADDR_PtoV1);///PA
   *(volatile size_t *)(PA_DISK+ADDR_PtoV2+DISK_SECTOR_OFFSET) = sector_num;
   *(volatile size_t *)(PA_DISK+ADDR_PtoV2+DISK_STATUS_OFFSET) = DISK_W_BIT;
   while(!nonblock && (*(size_t *)(PA_DISK+ADDR_PtoV2+DISK_STATUS_OFFSET)|DISK_RW_MASK)==DISK_RW_MASK);///wait for pending
}
void write_block(size_t sector_num, void *buf){
   _write_block_(sector_num,buf,0);
}

unsigned char DISK_BUFF[SECTOR_SIZE];
void readdisk(size_t sector_num, ssize_t offset, void *buf, size_t len){
   unsigned char *addr;
   kprintf("sector_num:%x\noffset:%x\nlen:%u\nbuf:%x\n",sector_num,offset,len,buf);
   addr = buf;
   read_block(sector_num,DISK_BUFF);
   ++sector_num;
   while(len!=0){
      while(offset>=SECTOR_SIZE){//mem miss
         read_block(sector_num++,DISK_BUFF);
         offset-=SECTOR_SIZE;
      }
      *addr=DISK_BUFF[offset];
      ++offset;
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
   DMA_addr = *(size_t *)(PA_DISK+ADDR_PtoV2+DISK_DMA_OFFSET);
   sector = *(size_t *)(PA_DISK+ADDR_PtoV2+DISK_SECTOR_OFFSET);
   status = *(size_t *)(PA_DISK+ADDR_PtoV2+DISK_STATUS_OFFSET);
   size =  *(size_t *)(PA_DISK+ADDR_PtoV2+DISK_RW_MASK);
   kprintf("DMA:%x\nSector:%x\nStatus:%x\nSize:%x\n",DMA_addr,sector,status,size);
   return size;
}


int g = 6;//just a test not magic num

void change_global_data(void){
   kprintf("g = %d\n",g);
   kprintf("addr g = %x\n",&g);
   g = 8;//not a magic num
   kprintf("g = %d\n",g);
}

int main(void){
    unsigned char buff[1100];//just a test not a magic num
    int a;
   //while(1)
    reloc_data_bss();
    kputs("Hello there!\n");

    change_global_data();

    kprintf("Disk size: %x\n",disk_size());
    _read_block_(0,buff,1);
    kprintf("Data:%x\n",buff[0]);
    //read_block(0x4b800,buff); ///kern on disk
    //for(a=0;a<52;++a)
    //  kprintf("data is like %x\n",buff[a]);


    kprintf("MBR begin\n");
    execute_mbr();
    kprintf("MBR end\n");
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
    volatile unsigned char *pointer = (volatile unsigned char *)(PA_PRINTER+ADDR_PtoV2);
    for(; *buf != '\0';++buf)
        *pointer = *buf;

}

