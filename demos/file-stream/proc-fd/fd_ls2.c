#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/limits.h>   //linux constants aka PATH_MAX

void list_open_file_descriptors(pid_t pid) {
    char fd_path[PATH_MAX];
    char link_path[PATH_MAX];
    DIR *dir;
    struct dirent *entry;
    ssize_t link_len;

    // Truncate the path if it's too long
    snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd", pid);

    dir = opendir(fd_path);
    if (dir == NULL) {
        perror("Could not open file descriptor directory");
        return;
    }

    printf("Open File Descriptors for PID %d:\n", pid);

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (entry->d_name[0] == '.') continue;

        // Construct full path to symlink, allow truncation
        char full_path[PATH_MAX + sizeof(entry->d_name)];
        snprintf(full_path, sizeof(full_path), "%s/%s", fd_path, entry->d_name);

        // Read symlink, handling potential truncation
        link_len = readlink(full_path, link_path, sizeof(link_path) - 1);
        if (link_len != -1) {
            link_path[link_len] = '\0';
            printf("FD %s: %s\n", entry->d_name, link_path);
        } else {
            // Optional: print error if readlink fails
            perror("Could not read symlink");
        }
    }

    closedir(dir);
}

#define DEFAULT_F_NAME "../war-and-peace.txt"
int main(int argc, char *argv[]) {
    pid_t pid;
    char *fname;
    int fd;

    //Use current PID
    pid = getpid();

    fname = (argc > 1) ? argv[1] : DEFAULT_F_NAME;

    printf("fd list prior to opening %s\n\n", fname);
    list_open_file_descriptors(pid);
    
    fd = open(fname, O_RDONLY);
    printf("\nfd last after opening file: %d\n\n", fd);

    list_open_file_descriptors(pid);

    close(fd);
    return 0;
}