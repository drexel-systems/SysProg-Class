#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IN_FILE_NAME "./war-and-peace.txt"
#define OUT_FILE_NAME "./war-and-peace-copy.txt"

#define DEFAULT_BUFF_SZ 1

FILE *open_input_file(char *fname);
FILE *open_output_file(char *fname);

/*
 *   open_input_file
 *      fname:  full file path of the file to open
 *
 *   Since we are opening the input file we want to not use
 *   any flags that would create the file if it did not exist
 *
 *   returns file descriptor of input file or error code
 */
FILE *open_input_file(char *fname)
{
    FILE *fd;

    fd = fopen(fname, "r");
    if (fd == NULL)
    {
        perror("file open error");
        return fd; // return the error
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
FILE *open_output_file(char *fname)
{
    FILE *fd;

    fd = fopen(fname, "w");
    if (fd == NULL)
    {
        perror("file open error");
        return fd; // return the error
    }

    return fd;
}

int main(int argc, char *argv[])
{
    int bsz = DEFAULT_BUFF_SZ;
    char *buff;
    FILE *in_fd;
    FILE *out_fd;
    int i_read, i_written;
    int b_copied = 0;

    if (argc >= 2)
    {

        // handle args
        if (strcmp(argv[1], "-h") == 0)
        {
            printf("usage: %s [num] | [-h]\n", argv[0]);
            printf("where:\n");
            printf("\t num: buffer/copy size [default is %d]\n",
                   DEFAULT_BUFF_SZ);
            printf("\t  -h: prints this help message\n");
            exit(0);
        }

        bsz = atoi(argv[1]);
        // reset to default if arg[1] cant be converted to an int
        bsz = (bsz == 0) ? DEFAULT_BUFF_SZ : bsz;
    }

    buff = malloc(bsz);
    if (buff == NULL)
    {
        perror("buff allocation failure");
        exit(1);
    }

    printf("Copying from %s to %s, copy rate = %d byte(s) at a time\n",
           IN_FILE_NAME, OUT_FILE_NAME, bsz);

    // There are more efficent ways to do these things but I created
    // helper functions to show how flags and modes are setup;
    in_fd = open_input_file(IN_FILE_NAME);
    out_fd = open_output_file(OUT_FILE_NAME);

    if ((in_fd == NULL) || (out_fd == NULL))
    {
        printf("Either the input or output file could not be opened\n");
        exit(2);
    }

    // Loop to process the file
    while (1)
    {
        i_read = fread(buff, bsz, 1, in_fd);
        if (i_read == 0)
            break; // hit EOF
        if (i_read < 0)
        {
            perror("error reading input file");
            break;
        }

        // fwrite(source, item size, number of items, dest fd)
        i_written = fwrite(buff, bsz, 1, out_fd);
        if (i_written < 0)
        {
            perror("error writing output file");
            break;
        }

        // fwrite turns number of items written (not bytes), which should always be 1
        if (i_written != 1)
        {
            // note maybe a better way to handle this would be to
            // loop and keep trying, should not happen on a file more
            // likely on a network
            printf("error: wrote %d items, expected to write 1 item\n",
                   i_written);
            break;
        }
        b_copied++;

        // cheap progress bar
        // if ((b_copied % 100) == 0)
        //     printf(".");
    }

    printf("\nDone - copied %d bytes\n", b_copied);

    // Lets not forget to free clean up;
    fclose(in_fd);
    fclose(out_fd);
    free(buff);
}