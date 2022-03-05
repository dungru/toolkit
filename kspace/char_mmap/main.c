#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096

int 
main (int argc, char **argv )
{
    int fd;
    char * address = NULL;
    fd = open("/dev/char_ks2us_dev", O_RDWR);
    if(fd < 0) {
        perror("open");
        return -1;
    }
 
    address = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (address == MAP_FAILED) {
        perror("mmap");
        return -1;
    }
    
    printf("initial ptr %p\n", address);
    printf("initial message: %s\n", address);
    address[0] = 'f';
    printf("changed message: %s\n", address);
    
    munmap(address, PAGE_SIZE);
    close(fd);
    return 0;

}
