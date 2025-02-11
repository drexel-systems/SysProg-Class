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


struct aiocb *async_read(int fd, char *buff, int bytes){
    struct aiocb *aio = malloc(sizeof(struct aiocb));
    if(aio == NULL)
        return NULL;

    memset(aio, 0, sizeof(struct aiocb));

    aio->aio_buf = buff;
    aio->aio_fildes = fd;
    aio->aio_nbytes = bytes;
    aio->aio_offset = 0; //start at beginning

    int result = aio_read(aio);
    if (result < 0){
        free(aio);
        return NULL;
    }

    return aio;
}

int main(int argc, char *argv[]){
    int bsz = DEFAULT_BUFF_SZ;
    char *buff;
    int in_fd;
    int b_read;
    int b_copied = 0;
    int ret;

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

    printf("Reading from %s asynchronously %d byte(s) at a time\n",
        IN_FILE_NAME, bsz);

    //There are more efficent ways to do these things but I created
    //helper functions to show how flags and modes are setup;
    in_fd = open_input_file(IN_FILE_NAME);

    if(in_fd < 0){
        printf("The input file could not be opened\n");
        printf("input fd = %d\n", in_fd);
        exit(2);
    }

    struct aiocb *aio = async_read(in_fd, buff, bsz);

    unsigned long work_counter = 0;

    while(aio_error(aio) == EINPROGRESS){
        //useful work could go here
        work_counter++;
    }

    ret = aio_return(aio);
    printf("AIO return %d\n", ret);
    perror("aio");


    printf("\nDone - useful work counter is: %ld\n", work_counter);

    //Lets not forget to free clean up;
    close(in_fd);
    free(buff);
}