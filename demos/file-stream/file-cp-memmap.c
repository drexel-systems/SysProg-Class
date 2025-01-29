#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define IN_FILE_NAME "./war-and-peace.txt"
#define OUT_FILE_NAME "./war-and-peace-copy.txt"

int main(int argc, char *argv[])
{
    int in_fd, out_fd;
    struct stat sb;
    char *src, *dest;
    int bcopy_sz = -1;

    printf("Copying from %s to %s using memory-mapped files\n", IN_FILE_NAME, OUT_FILE_NAME);

    if (argc >= 2){
        int bsz = atoi(argv[1]);
        // reset to default if arg[1] cant be converted to an int
        bcopy_sz = (bsz == 0) ? -1 : bsz;

        if(bcopy_sz != -1){
            printf("Using a copy window of %d byte(s)\n", bcopy_sz);
        }
    }

    // Open the input file
    in_fd = open(IN_FILE_NAME, O_RDONLY);
    if (in_fd == -1)
    {
        perror("Error opening input file");
        exit(1);
    }

    // Get the size of the input file
    if (fstat(in_fd, &sb) == -1)
    {
        perror("Error getting input file size");
        close(in_fd);
        exit(1);
    }

    printf("Input file has size of %ld bytes\n", sb.st_size);

    // Memory-map the input file
    src = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, in_fd, 0);
    if (src == MAP_FAILED)
    {
        perror("Error memory-mapping input file");
        close(in_fd);
        exit(1);
    }

    // Open the output file
    out_fd = open(OUT_FILE_NAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (out_fd == -1)
    {
        perror("Error opening output file");
        munmap(src, sb.st_size);
        close(in_fd);
        exit(1);
    }

    // Ensure the output file is the same size as the input file
    if (ftruncate(out_fd, sb.st_size) == -1)
    {
        perror("Error setting output file size");
        munmap(src, sb.st_size);
        close(in_fd);
        close(out_fd);
        exit(1);
    }

    // Memory-map the output file
    dest = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, 0);
    if (dest == MAP_FAILED)
    {
        perror("Error memory-mapping output file");
        munmap(src, sb.st_size);
        close(in_fd);
        close(out_fd);
        exit(1);
    }

    if (bcopy_sz == -1){
        // Copy the input file to the output file using memory (one shot)
        memcpy(dest, src, sb.st_size);
    } else {
        //copy by chunks to see if the performance matters
        char *dptr = dest;
        char *sptr = src;
        int  remaining = sb.st_size;
        int  copy_chunks = 0;
         
        while (remaining > 0){
            int  cpy_window = (bcopy_sz < remaining)? bcopy_sz : remaining;
            memcpy(dptr, sptr, cpy_window);
            sptr += cpy_window;
            dptr += cpy_window;
            remaining -= cpy_window;
            copy_chunks++;
        }
        printf("copied src to dest in memory using %d chunks\n", copy_chunks);
    }

    // Clean up
    munmap(src, sb.st_size);
    munmap(dest, sb.st_size);
    close(in_fd);
    close(out_fd);

    printf("Done - copied %ld bytes\n", sb.st_size);
    return 0;
}
