#include <sys/types.h>
#include <stddef.h>
#include <elf.h>

struct chs {
	unsigned char	head;
	unsigned char	sector:6;
	unsigned char	cylinder_hi:2;
	unsigned char	cylinder_lo;
};

struct mbr_part_entry {
	uint8_t		status;
#define BOOTABLE	0x80
#define INACTIVE	0x00
	struct chs	first_sector_chs;
	uint8_t		type;
	struct chs	last_sector_chs;
	uint32_t	first_sector_lba;
	uint32_t	sector_count;
};

typedef ssize_t (*readdisk_func_t)(size_t, size_t, void *, size_t);
typedef void (*entry_t)(void);
typedef int (*kprintf_t)(char *fmt,...);

#define mbr_addr 0x80001000
#define PART0_ENTRY_OFFSET	446
#define PART0_ENTRY		((unsigned long)mbr_addr + PART0_ENTRY_OFFSET)

/* The first 446 bytes contains this piece of code. */
void boot(readdisk_func_t readdisk,kprintf_t kprintf)
{
	struct elfhdr eh;
	struct elf_phdr ph;
	Elf_Half i;
	entry_t entry;
	Elf_Off pos = 0;
	/*
	 * You can't use the code below to obtain LBA.
	 * Try uncommenting the following to see what will happen.
	 */
	unsigned long addr = (unsigned long)mbr_addr + PART0_ENTRY_OFFSET
	    + sizeof(struct mbr_part_entry)
	    + MEMBER_OFFSET(struct mbr_part_entry, first_sector_lba);
	uint32_t lba = *(uint16_t *)(addr + 2);
	uintptr_t seg;
	lba = (lba << 16) + *(uint16_t *)addr;
	/* Read the ELF header first */
	readdisk(lba, pos, &eh, sizeof(eh));
        ///eq (*readdisk)(...)
	pos = eh.e_phoff;
        ///kprintf("r %x %x %x %x\n",&eh,*(unsigned char*)&eh,*((unsigned char *)&eh+16),0);
	for (i = 0; i < eh.e_phnum; ++i) {
		readdisk(lba, pos, &ph, sizeof(ph));
		if (ph.p_type == PT_LOAD) {
			seg = (void *)ph.p_vaddr;
			readdisk(lba, ph.p_offset, seg, ph.p_filesz);
		}
		pos += eh.e_phentsize;
	}
	entry = (entry_t)(eh.e_entry);
	(*entry)();
}
