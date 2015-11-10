#ifndef _MACH_CONFIG_H
#define _MACH_CONFIG_H

/*firmware-main*/
#define PA_DISK 0x1f000020
#define PA_PRINTER 0x1f000010

#define MBR_INITIAL_ADDR 0x80001000

#define DISK_DMA_OFFSET 0
#define DISK_SECTOR_OFFSET 4
#define DISK_STATUS_OFFSET 8
#define DISK_SIZE_OFFSET 12
#define DISK_RW_MASK 0x03
#define DISK_R_BIT 0x01
#define DISK_W_BIT 0x02


/*firmware-entry*/
#define FIRMWARE_STACK_ADDR  0x8f000000

#endif