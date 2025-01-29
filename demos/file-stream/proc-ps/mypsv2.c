#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_PATH 256
#define MAX_CMDLINE 1024

void read_cmdline(int pid, char *cmdline) {
    char path[MAX_PATH];
    FILE *fp;

    // Construct path to cmdline file
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
    
    // Open cmdline file
    fp = fopen(path, "r");
    if (!fp) {
        cmdline[0] = '\0';
        return;
    }

    // Read cmdline, replace null bytes with spaces
    size_t bytes_read = fread(cmdline, 1, MAX_CMDLINE - 1, fp);
    cmdline[bytes_read] = '\0';
    
    for (size_t i = 0; i < bytes_read; i++) {
        if (cmdline[i] == '\0') {
            cmdline[i] = ' ';
        }
    }

    fclose(fp);
}

void read_executable_name(int pid, char *exe_name) {
    char path[MAX_PATH];
    char link[MAX_PATH];
    ssize_t len;

    // Construct path to exe symlink
    snprintf(path, sizeof(path), "/proc/%d/exe", pid);
    
    // Read symlink to get executable name
    len = readlink(path, link, sizeof(link) - 1);
    if (len != -1) {
        link[len] = '\0';
        
        // Extract just the filename from full path
        char *base = strrchr(link, '/');
        strcpy(exe_name, base ? base + 1 : link);
    } else {
        exe_name[0] = '\0';
    }
}

void scan_proc_directory() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Error opening /proc directory");
        return;
    }

    printf("PID\tExecutable\tCommand Line\n");

    while ((entry = readdir(dir)) != NULL) {
        // Check if entry is a PID
        int is_pid = 1;
        for (int i = 0; entry->d_name[i] != '\0'; i++) {
            if (!isdigit(entry->d_name[i])) {
                is_pid = 0;
                break;
            }
        }

        if (is_pid) {
            int pid = atoi(entry->d_name);
            char exe_name[MAX_PATH] = {0};
            char cmdline[MAX_CMDLINE] = {0};

            read_executable_name(pid, exe_name);
            read_cmdline(pid, cmdline);

            // Only print if we found a meaningful entry
            if (exe_name[0] != '\0' || cmdline[0] != '\0') {
                printf("%d\t%s\t\t%.40s\n", pid, exe_name, cmdline);
            }
        }
    }

    closedir(dir);
}

int main() {
    scan_proc_directory();
    return 0;
}