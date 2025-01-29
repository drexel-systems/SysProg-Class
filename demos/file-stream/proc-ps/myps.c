#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void scan_proc_directory() {
    DIR *dir;
    struct dirent *entry;

    // Open the /proc directory
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening /proc directory");
        return;
    }

    printf("Current Process IDs:\n");

    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Check if entry name is a number (PID)
        int is_pid = 1;
        for (int i = 0; entry->d_name[i] != '\0'; i++) {
            if (!isdigit(entry->d_name[i])) {
                is_pid = 0;
                break;
            }
        }

        // Print PID if it's a numeric directory
        if (is_pid) {
            printf("%s\n", entry->d_name);
        }
    }

    // Close directory
    closedir(dir);
}

int main() {
    scan_proc_directory();
    return 0;
}