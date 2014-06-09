#include<stdio.h>
#include<stdlib.h>

#define EI_NIDENT 16
#define ELF32_ST_BIND(i)	((i)>>4)
#define ELF32_ST_TYPE(i)	((i)&0xf)
#define ELF32_ST_INFO(b,t) 	(((b)<<4)+((t)&0xf))

typedef short int Elf32_Half; 
typedef unsigned int Elf32_Word;
typedef unsigned int Elf32_Off;
typedef unsigned int Elf32_Addr;

typedef struct { 
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half e_type;
	Elf32_Half e_machine;
	Elf32_Word e_version;
	Elf32_Addr e_entry;
	Elf32_Off e_phoff;
	Elf32_Off e_shoff;
	Elf32_Word e_flags;
	Elf32_Half e_ehsize;
	Elf32_Half e_phentsize;
	Elf32_Half e_phnum;
	Elf32_Half e_shentsize;
	Elf32_Half e_shnum;
	Elf32_Half e_shstrndx;
} Elf32_Ehdr; 

typedef struct {
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off  sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct {
	Elf32_Word p_type;
	Elf32_Off p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} Elf32_Phdr;

typedef struct {
	Elf32_Word st_name;
	Elf32_Addr st_value;
	Elf32_Word st_size;
	unsigned char st_info;
	unsigned char st_other;
	Elf32_Half st_shndx;
} Elf32_Sym;

int main()
{
	int i;
	int j;
	char a[100];
	char b[100];
	FILE *fp;
	void *fphdr;
	void *shdr;
	void *phdr;
	void *symtab;
	void *dynsymtab;
	ssize_t size;
	Elf32_Ehdr elfhdr;
	
	fp = fopen("a.out","r");
	fphdr = malloc(sizeof(Elf32_Ehdr));
	size = fread(fphdr, sizeof(Elf32_Ehdr),1, fp);
	printf("\n* Elf header :\n");	
	printf("* Magic :");	
	for (i=0; i<16;i++)
	{	
		elfhdr.e_ident[i] = ((Elf32_Ehdr*)fphdr)->e_ident[i];
		printf(" %02x ", elfhdr.e_ident[i]);	
	}
	switch(elfhdr.e_ident[4])
	{
		case 0: printf("\n* Class				: Invalid Class\n"); break;
		case 1: printf("\n* Class				: ELF32\n"); break;
		case 2: printf("\n* Class				: ELF64\n"); break;
		default: printf("\n* Class				: Invalid Class Type\n"); break;
	}
	switch(elfhdr.e_ident[5])
	{
		case 0: printf("* Data				: Invalid Data Encoding\n"); break;
		case 1: printf("* Data				: 2's complement, Little Endian\n"); break;
		case 2: printf("* Data				: 2's complement, Big Endian\n"); break;
		default: printf("* Data				: Invalid Data Encoding Type\n"); break;
	}
	elfhdr.e_ident[6] = ((Elf32_Ehdr*)fphdr)->e_ident[6];
	printf("* Version			: %d\n", elfhdr.e_ident[6]); 
	elfhdr.e_type = ((Elf32_Ehdr*)fphdr)->e_type;
	switch(elfhdr.e_type)
	{
		case 0: printf("* Type				: No Type\n"); break;
		case 1: printf("* Type				: Relocatable\n"); break;
		case 2: printf("* Type				: Executable\n"); break;
		case 3: printf("* Type				: Shared Object\n"); break;
		case 4: printf("* Type				: Core File\n"); break;
		default: printf("* Type				: Invalid File Type\n"); break;
	}	
	
	elfhdr.e_machine = ((Elf32_Ehdr*)fphdr)->e_machine;
	switch(elfhdr.e_machine)
	{
		case 0: printf("* Machine			: No Machine\n"); break;
		case 1: printf("* Machine			: AT&T\n"); break;
		case 2: printf("* Machine			: SPARC\n"); break;
		case 3: printf("* Machine			: INTEL 80386\n"); break;
		case 4: printf("* Machine			: Motorola 68000\n"); break;
		case 5: printf("* Machine			: Motorola 88000\n"); break;
		case 7: printf("* Machine			: INTEL 80860\n"); break;
		case 8: printf("* Machine			: MIPS RS3000\n"); break;
		default: printf("* Machine			: Invalid Machine Type\n"); break;
	}
	elfhdr.e_version = ((Elf32_Ehdr*)fphdr)->e_version;
	switch(elfhdr.e_version)
	{
		case 0: printf("* Version			: 0x0\n"); break;
		case 1: printf("* Version			: 0x1\n"); break;
		default: printf("* Version			: Invalid Version Type\n"); break;
	}
	elfhdr.e_entry = ((Elf32_Ehdr*)fphdr)->e_entry;
	printf("* Entry Point Address		: Ox%x\n", elfhdr.e_entry); 

	elfhdr.e_phoff = ((Elf32_Ehdr*)fphdr)->e_phoff;
	printf("* Start of Program Headers	: %d (bytes into file)\n", elfhdr.e_phoff); 
	elfhdr.e_shoff = ((Elf32_Ehdr*)fphdr)->e_shoff;
	printf("* Start of Section Headers	: %d (bytes into file)\n", elfhdr.e_shoff); 
	
	printf("* Flags				: 0x0\n");	
	printf("* Size of this Header   	: %d (bytes)\n", sizeof(Elf32_Ehdr));	
	
	elfhdr.e_phentsize = ((Elf32_Ehdr*)fphdr)->e_phentsize;
	printf("* Size of Program Headers	: %d (bytes)\n", elfhdr.e_phentsize); 
	elfhdr.e_phnum = ((Elf32_Ehdr*)fphdr)->e_phnum;
	printf("* Number of Program Headers	: %d\n", elfhdr.e_phnum); 
	
	elfhdr.e_shentsize = ((Elf32_Ehdr*)fphdr)->e_shentsize;
	printf("* Size of Section Headers	: %d (bytes)\n", elfhdr.e_shentsize); 
	elfhdr.e_shnum = ((Elf32_Ehdr*)fphdr)->e_shnum;
	printf("* Number of Section Headers	: %d\n", elfhdr.e_shnum); 

	elfhdr.e_shstrndx = ((Elf32_Ehdr*)fphdr)->e_shstrndx;
	printf("* Section Header str table index: %d\n\n", elfhdr.e_shstrndx); 
	
	fseek(fp, elfhdr.e_shoff, SEEK_SET);
	shdr = malloc(sizeof(Elf32_Shdr)*elfhdr.e_shnum);
	size = fread(shdr, sizeof(Elf32_Shdr),elfhdr.e_shnum, fp);

	printf("\n* Section headers :\n");	
	printf("* [Nr]\tType\t\tAddress\t\tOffset\tSize\tES\tFlg\tLink\tIF\tAL\tName\n");	
	for(i=0; i < elfhdr.e_shnum;i++)
	{
		j = 0;
		printf("* [%2d]",i); 
		fseek(fp,((((Elf32_Shdr*)shdr)[elfhdr.e_shstrndx]).sh_offset + (((Elf32_Shdr*)shdr)[i]).sh_name),SEEK_SET);
		do
		{
			a[j] = getc(fp);		
		}while('\0' != a[j++]);
		
		
		switch( (((Elf32_Shdr*)shdr)[i]).sh_type)
		{
			case 0: printf("\tNULL\t"); break;
			case 1: printf("\tPROGBITS"); break;
			case 2: printf("\tSYMTAB\t");break;
			case 3: printf("\tSTRTAB\t"); break;
			case 4: printf("\tRELA\t"); break;
			case 5: printf("\tHASH\t"); break;
			case 6: printf("\tDYNAMIC\t"); break;
			case 7: printf("\tNOTE\t"); break;
			case 8: printf("\tNOBITS\t");break;
			case 9: printf("\tREL\t"); break;
			case 10: printf("\tSHLIB"); break;
			case 11: printf("\tDYNSYM\t"); break;
			case 0x70000000: printf("\tLOPROC"); break;
			case 0x7fffffff: printf("\tHIPROC"); break;
			case 0x80000000: printf("\tLOUSER"); break;
			case 0xffffffff: printf("\tHIUSER"); break;
			default:printf("\tINVALID\t");break;	
		}
		printf("\t%08x", (((Elf32_Shdr*)shdr)[i]).sh_addr);	
		printf("\t%05x", (((Elf32_Shdr*)shdr)[i]).sh_offset);	
		printf("\t%05x", (((Elf32_Shdr*)shdr)[i]).sh_size);	
		printf("\t%02x", (((Elf32_Shdr*)shdr)[i]).sh_entsize);
		printf("\t");	
		if((((Elf32_Shdr*)shdr)[i]).sh_flags & 0x1)
			printf("W");	
		if((((Elf32_Shdr*)shdr)[i]).sh_flags & 0x2)
			printf("A");	
		if((((Elf32_Shdr*)shdr)[i]).sh_flags & 0x4)
			printf("X");	
		if((((Elf32_Shdr*)shdr)[i]).sh_flags & 0xf0000000)
			printf("MS");	
		printf("\t%d", (((Elf32_Shdr*)shdr)[i]).sh_link);	
		printf("\t%d", (((Elf32_Shdr*)shdr)[i]).sh_info);	
		printf("\t%d", (((Elf32_Shdr*)shdr)[i]).sh_addralign);	
		printf("\t%s\n", a);
	}
	
	fseek(fp, elfhdr.e_phoff, SEEK_SET);
	phdr = malloc(sizeof(Elf32_Phdr)*elfhdr.e_phnum);
	size = fread(phdr, sizeof(Elf32_Phdr),elfhdr.e_phnum, fp);
	
	printf("\n* Program headers :\n");	
	printf("Type\t\tOffset\t\tVirtAddr\tPhysAddr\tFileSiz\tMemSiz\tAlign\n");	
	for(i=0; i < elfhdr.e_phnum;i++)
	{
		switch((((Elf32_Phdr*)phdr)[i]).p_type)
		{
			case 0: printf("NULL\t"); break;
			case 1: printf("LOAD\t"); break;
			case 2: printf("DYNAMIC\t");break;
			case 3: printf("INTERP\t"); break;
			case 4: printf("NOTE\t"); break;
			case 5: printf("SHLIB\t"); break;
			case 6: printf("PHDR\t"); break;
			case 0x70000000: printf("LOPROC\t"); break;
			case 0x7fffffff: printf("HIPROC\t"); break;
			default:printf("INVALID\t");break;	
		}
		printf("\t0x%08x", (((Elf32_Phdr*)phdr)[i]).p_offset);	
		printf("\t0x%08x\t", (((Elf32_Phdr*)phdr)[i]).p_vaddr);	
		printf("0x%08x\t", (((Elf32_Phdr*)phdr)[i]).p_paddr);	
		printf("0x%05x", (((Elf32_Phdr*)phdr)[i]).p_filesz);	
		printf("\t0x%05x", (((Elf32_Phdr*)phdr)[i]).p_memsz);	
//		printf("\t%x", (((Elf32_Phdr*)phdr)[i]).p_flags);	
		printf("\t0x%x\n", (((Elf32_Phdr*)phdr)[i]).p_align);	
		
	}
	printf("\nSymbol table '.dyntab' contains %d entries:\n", (((Elf32_Shdr*)shdr)[6]).sh_size/sizeof(Elf32_Sym));	
	fseek(fp, (((Elf32_Shdr*)shdr)[6]).sh_offset, SEEK_SET);
	dynsymtab = malloc(sizeof(Elf32_Sym)*(((Elf32_Shdr*)shdr)[6]).sh_size);
	size = fread(dynsymtab, sizeof(Elf32_Sym),(((Elf32_Shdr*)shdr)[6]).sh_size/sizeof(Elf32_Sym), fp);
	
	printf(" Num:\tValue\t\tSize\tType\tBind\tVis\tNdx\tName\n");	
	for(i=0; i < (((Elf32_Shdr*)shdr)[6]).sh_size/sizeof(Elf32_Sym);i++)
	{
		j = 0;
		printf("* [%2d]",i); 
		fseek(fp,((((Elf32_Shdr*)shdr)[(((Elf32_Shdr*)shdr)[6]).sh_link]).sh_offset + (((Elf32_Sym*)dynsymtab)[i]).st_name),SEEK_SET);
		do
		{
			b[j] = getc(fp);		
		}while('\0' != b[j++]);
		printf("\t%08x", (((Elf32_Sym*)dynsymtab)[i]).st_value);	
		printf("\t%d", (((Elf32_Sym*)dynsymtab)[i]).st_size);	
		switch(ELF32_ST_TYPE((((Elf32_Sym*)dynsymtab)[i]).st_info))
		{
			case 0: printf("\tNOTYPE\t"); break;
			case 1: printf("\tOBJECT\t"); break;
			case 2: printf("\tFUNC\t");break;
			case 3: printf("\tSECTION\t");break;
			case 4: printf("\tFILE\t");break;
			case 13: printf("\tLOPROC\t");break;
			case 15: printf("\tHIPROC\t"); break;
			default: printf("\tINVALID TYPE\t"); break;
		}
		switch(ELF32_ST_BIND((((Elf32_Sym*)dynsymtab)[i]).st_info))
		{
			case 0: printf("LOCAL\t"); break;
			case 1: printf("GLOBAL\t"); break;
			case 2: printf("WEAK\t");break;
			case 13: printf("LOPROC\t");break;
			case 15: printf("HIPROC\t"); break;
			default: printf("INVALID TYPE\t"); break;
		}
		printf("DEFAULT\t"); 
		if((((Elf32_Sym*)dynsymtab)[i]).st_shndx < 0)
			printf("ABS\t");	
		if((((Elf32_Sym*)dynsymtab)[i]).st_shndx == 0)
			printf("UND\t");	
		if((((Elf32_Sym*)dynsymtab)[i]).st_shndx > 0)
			printf("%d\t", (((Elf32_Sym*)dynsymtab)[i]).st_shndx);	
		printf("%s\n", b);
			
	}
	printf("\n\nSymbol table '.symtab' contains %d entries:\n", (((Elf32_Shdr*)shdr)[28]).sh_size/sizeof(Elf32_Sym));	
	fseek(fp, (((Elf32_Shdr*)shdr)[28]).sh_offset, SEEK_SET);
	symtab = malloc(sizeof(Elf32_Sym)*(((Elf32_Shdr*)shdr)[28]).sh_size);
	size = fread(symtab, sizeof(Elf32_Sym),(((Elf32_Shdr*)shdr)[28]).sh_size/sizeof(Elf32_Sym), fp);
	
	printf(" Num:\tValue\t\tSize\tType\tBind\tVis\tNdx\tName\n");	
	for(i=0; i < (((Elf32_Shdr*)shdr)[28]).sh_size/sizeof(Elf32_Sym);i++)
	{
		j = 0;
		printf("* [%2d]",i); 
		fseek(fp,((((Elf32_Shdr*)shdr)[(((Elf32_Shdr*)shdr)[28]).sh_link]).sh_offset + (((Elf32_Sym*)symtab)[i]).st_name),SEEK_SET);
		do
		{
			b[j] = getc(fp);		
		}while('\0' != b[j++]);
		printf("\t%08x", (((Elf32_Sym*)symtab)[i]).st_value);	
		printf("\t%d", (((Elf32_Sym*)symtab)[i]).st_size);	
		switch(ELF32_ST_TYPE((((Elf32_Sym*)symtab)[i]).st_info))
		{
			case 0: printf("\tNOTYPE\t"); break;
			case 1: printf("\tOBJECT\t"); break;
			case 2: printf("\tFUNC\t");break;
			case 3: printf("\tSECTION\t");break;
			case 4: printf("\tFILE\t");break;
			case 13: printf("\tLOPROC\t");break;
			case 15: printf("\tHIPROC\t"); break;
			default: printf("\tINVALID TYPE\t"); break;
		}
		switch(ELF32_ST_BIND((((Elf32_Sym*)symtab)[i]).st_info))
		{
			case 0: printf("LOCAL\t"); break;
			case 1: printf("GLOBAL\t"); break;
			case 2: printf("WEAK\t");break;
			case 13: printf("LOPROC\t");break;
			case 15: printf("HIPROC\t"); break;
			default: printf("INVALID TYPE\t"); break;
		}
		printf("DEFAULT\t"); 
		if((((Elf32_Sym*)symtab)[i]).st_shndx < 0)
			printf("ABS\t");	
		if((((Elf32_Sym*)symtab)[i]).st_shndx == 0)
			printf("UND\t");	
		if((((Elf32_Sym*)symtab)[i]).st_shndx > 0)
			printf("%d\t", (((Elf32_Sym*)symtab)[i]).st_shndx);	
		printf("%s\n", b);
			
	}
		printf("\n\n");

	return 0;
}
