#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <aio.h>
#include <errno.h>

#define IN_FILE_NAME "./war-and-peace.txt"
#define OUT_FILE_NAME "./war-and-peace-copy.txt"

#define DEFAULT_BUFF_SZ 1


int open_input_file(char *fname);
int open_output_file(char *fname);

/*
 *   open_input_file
 *      fname:  full file path of the file to open
 * 
 *   Since we are opening the input file we want to not use
 *   any flags that would create the file if it did not exist
 * 
 *   returns file descriptor of input file or error code
 */
int open_input_file(char *fname){
    int fd;

    //see man 2 open for documentation and how to setup the flags
    //flags specified in teh #include<bits/fcntl-linux.h> header 
    //file
    int flags = O_RDONLY;

    fd = open(fname, flags);
    if (fd < 0){
        perror("file open error");
        return fd;  //return the error
    }

    return fd;
}

/*
 *   open_output_file
 *      fname:  full file path of the file to open
 * 
 *   Since we are opening the output file we want to ensure
 *   our flags:
 * 
 *      - Open the file in write-only mode
 *      - Create the file if it does not exist
 *      - If it exists open using TRUNCATE which basically 
 *        deletes the previous contents, sets the internal
 *        file pointer to the zero byte
 *      - Set the file permission mode for this file
 * 
 *   returns file descriptor of input file or error code
 */
int open_output_file(char *fname){
    int fd;

    //see man 2 open for documentation and how to setup the flags
    //flags specified in teh #include<bits/fcntl-linux.h> header 
    //file.  This is a bitfield so we logically OR all of the 
    //flags that we want to set
    int flags = O_WRONLY | O_CREAT | O_TRUNC;

    //since we may be creating a file we want to provide a mode
    //we can provide it as an octal constant/ int mode = 0644
    //or use constants that might make it more readable
    //see: https://man7.org/linux/man-pages/man2/open.2.html
    //flag constants in #define<sys/stat.h>
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    fd = open(fname, flags, mode);
    if (fd < 0){
        perror("file open error");
        return fd;  //return the error
    }

    return fd;
}

struct aiocb *async_copy(int infd, int outfd, char *buff, int bsz){
    struct aiocb *aio = malloc(sizeof(struct aiocb));
    if (aio == NULL)
        return NULL;

    memset(aio, 0, malloc(sizeof(struct aiocb)));

    aio->aio_buf = buff;
    aio->aio_fildes = infd;

}

int main(int argc, char *argv[]){
    int bsz = DEFAULT_BUFF_SZ;
    char *buff;
    int in_fd, out_fd;
    int b_read, b_written;
    int b_copied = 0;

    unsigned long work_simulator = 0;

    if (argc >= 2){

        //handle args
        if (strcmp(argv[1], "-h") == 0){
            printf("usage: %s [num] | [-h]\n", argv[0]);
            printf("where:\n");
            printf("\t num: buffer/copy size [default is %d]\n",  
                DEFAULT_BUFF_SZ);
            printf("\t  -h: prints this help message\n");
            exit(0);
        }


        bsz = atoi(argv[1]);
        //reset to default if arg[1] cant be converted to an int
        bsz = (bsz == 0) ? DEFAULT_BUFF_SZ : bsz; 
    }

    buff = malloc(bsz);
    if (buff == NULL){
        perror("buff allocation failure");
        exit(1);
    }

    printf("Copying from %s to %s, copy rate = %d byte(s) at a time\n",
        IN_FILE_NAME, OUT_FILE_NAME, bsz);

    //There are more efficent ways to do these things but I created
    //helper functions to show how flags and modes are setup;
    in_fd = open_input_file(IN_FILE_NAME);
    out_fd = open_output_file(OUT_FILE_NAME);

    if((in_fd < 0) || (out_fd < 0)){
        printf("Either the input or output file could not be opened\n");
        printf("input fd = %d; output fd = %d\n", in_fd, out_fd);
        exit(2);
    }

    struct aiocb *aio = async_copy(in_fd, out_fd, buff, bsz);

    while(aio_error(aio) == EINPROGRESS){
        work_simulator++;
    }

    //Loop to process the file
    while(1){
        b_read = read(in_fd, buff, bsz);
        if (b_read == 0)
            break;      //hit EOF
        if (b_read < 0){
            perror("error reading input file");
            break;
        }

        b_written = write(out_fd, buff, bsz);
        if (b_read < 0){
            perror("error writing output file");
            break;
        }
        if (b_written != bsz){
            //note maybe a better way to handle this would be to 
            //loop and keep trying, should not happen on a file more
            //likely on a network
            printf("error: wrote %d bytes, expected to write %d bytes\n",
                b_written, bsz);
            break;
        }
        b_copied++;

        //cheap progress bar
        //if ((b_copied % 100) == 0)
        //    printf(".");
    }

    printf("\nDone - copied %d bytes\n", b_copied);

    //Lets not forget to free clean up;
    close(in_fd);
    close(out_fd);
    free(buff);
}