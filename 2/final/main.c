#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <sys/mman.h>


#include <sys/types.h>
#include <unistd.h>

// Format of an ELF executable file

#define ELF_MAGIC 0x464C457FU  // "\x7FELF" in little endian

// File header
struct elfhdr {
  unsigned int magic;  // must equal ELF_MAGIC
  unsigned char elf[12];
  unsigned short type;
  unsigned short machine;
  unsigned int version;
  unsigned int entry;
  unsigned int phoff;
  unsigned int shoff;
  unsigned int flags;
  unsigned short ehsize;
  unsigned short phentsize;
  unsigned short phnum;
  unsigned short shentsize;
  unsigned short shnum;
  unsigned short shstrndx;
};

// Program section header
struct proghdr {
  unsigned int type;
  unsigned int off;
  unsigned int vaddr;
  unsigned int paddr;
  unsigned int filesz;
  unsigned int memsz;
  unsigned int flags;
  unsigned int align;
};

// Values for Proghdr type
#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4

int main(int argc, char* argv[]) {
    struct elfhdr elf;
    struct proghdr ph;
    int (*sum)(int a, int b);
    void *entry = NULL;
    int ret; 

    /* Add your ELF loading code here */
    FILE * efile;
    void * code_va;

    efile = fopen(argv[1],"rb");
    //efile = open(argv[1], O_RDONLY);
    
    if (efile == NULL)
    {
        perror("open");
        exit(1);
    }

    fread(&elf,sizeof(elf),1, efile);
    //read(efile, &elf, sizeof(elf));
    //print_elfhdr(&elf);
    
    fseek(efile,elf.phoff, SEEK_SET);
    //lseek(efile, elf.phoff, SEEK_SET);

    fread(&ph,sizeof(ph),1,efile);
    //read(efile, &ph, sizeof(ph));

    //print_proghdr(&ph);
   
    code_va = mmap(NULL, ph.memsz, PROT_READ|PROT_WRITE|PROT_EXEC,
        MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
   
    if (code_va == MAP_FAILED)
    {
        perror("map");
        fclose(efile);
        exit(1);
    }

    if (ph.type==1)
    {
        fseek(efile,ph.off,SEEK_SET);
        fread(code_va,ph.filesz,1,efile);
        entry = code_va;
    }
    
    fclose(efile);

    if (entry != NULL) {
        sum = entry; 
        ret = sum(1, 2);
        printf("sum:%d\n", ret); 
    };
}


void print_proghdr(struct proghdr *ph)
{
    char *fields[] = {"type", "off", "vaddr", "paddr", "filesz", 
      "memsz", "flags", "align"};
    size_t sizes[] = {sizeof(ph->type), sizeof(ph->off),sizeof(ph->vaddr),
      sizeof(ph->paddr),sizeof(ph->filesz),sizeof(ph->memsz),sizeof(ph->flags),
    sizeof(ph->align)};

    char * c_ph = (char *) ph;

    for(size_t i = 0; i < sizeof(sizes)/sizeof(size_t); c_ph += sizes[i], i++)
    {
        printf("%s = ", fields[i]);
        for(size_t j = 0; j < sizes[i]; j++) 
          printf("%.2x ", c_ph[j]);
        printf("\n");
    }
}


void print_elfhdr(struct elfhdr *elf) {                                                                    
      char *fields[] = {"magic", "elf", "type",                                                       
                              "machine", "version", "entry",                                                
                                                    "phoff", "shoff", "flags",                                                    
                                                                          "ehsize", "phentsize", "phnum",                                               
                                                                                                "shentsize", "shnum", "shstrndx"};                                            
          size_t sizes[] = {sizeof(elf->magic), sizeof(elf->elf), sizeof(elf->type),                      
                                  sizeof(elf->machine), sizeof(elf->version), sizeof(elf->entry),               
                                                        sizeof(elf->phoff), sizeof(elf->shoff), sizeof(elf->flags),                   
                                                                              sizeof(elf->ehsize), sizeof(elf->phentsize), sizeof(elf->phnum),              
                                                                                                    sizeof(elf->shentsize), sizeof(elf->shnum), sizeof(elf->shstrndx)};           
              char *c_elf = (char *) elf;                                                                     
                  for(size_t i = 0; i < sizeof(sizes)/sizeof(size_t); c_elf += sizes[i], i++)                     
                        {                                                                                               
                                  printf("%s = ", fields[i]);                                                                 
                                          for(size_t j = 0; j < sizes[i]; j++)                                                        
                                                        printf("%.2x ", c_elf[j]);                                                              
                                                  printf("\n");                                                                               
                                                      }                                                                                               
}       


