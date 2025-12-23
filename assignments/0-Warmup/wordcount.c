#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [-l] [-w] [-c] [file ...]\n", program_name);
    fprintf(stderr, "Count lines, words, and characters in files or stdin\n");
    fprintf(stderr, "  -l    count lines\n");
    fprintf(stderr, "  -w    count words\n");
    fprintf(stderr, "  -c    count characters\n");
    fprintf(stderr, "  If no options specified, counts all three\n");
    fprintf(stderr, "  If no files specified, reads from stdin\n");
}

typedef struct {
    long lines;
    long words;
    long chars;
} Counts;

Counts count_stream(FILE *fp) {
    Counts counts = {0, 0, 0};
    int c;
    bool in_word = false;
    
    while ((c = fgetc(fp)) != EOF) {
        counts.chars++;
        
        if (c == '\n') {
            counts.lines++;
        }
        
        if (isspace(c)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            counts.words++;
        }
    }
    
    return counts;
}

void print_counts(Counts counts, bool show_lines, bool show_words, bool show_chars, const char *filename) {
    if (show_lines) {
        printf("%8ld", counts.lines);
    }
    if (show_words) {
        printf("%8ld", counts.words);
    }
    if (show_chars) {
        printf("%8ld", counts.chars);
    }
    if (filename) {
        printf(" %s", filename);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    bool show_lines = false;
    bool show_words = false;
    bool show_chars = false;
    bool any_option = false;
    int file_start = 1;
    
    // Parse options
    for (int i = 1; i < argc && argv[i][0] == '-'; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            show_lines = true;
            any_option = true;
        } else if (strcmp(argv[i], "-w") == 0) {
            show_words = true;
            any_option = true;
        } else if (strcmp(argv[i], "-c") == 0) {
            show_chars = true;
            any_option = true;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
        file_start = i + 1;
    }
    
    // If no options specified, show all
    if (!any_option) {
        show_lines = show_words = show_chars = true;
    }
    
    // No files specified, read from stdin
    if (file_start >= argc) {
        Counts counts = count_stream(stdin);
        print_counts(counts, show_lines, show_words, show_chars, NULL);
        return 0;
    }
    
    // Process files
    Counts total = {0, 0, 0};
    int num_files = 0;
    
    for (int i = file_start; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (!fp) {
            fprintf(stderr, "Error: cannot open file '%s'\n", argv[i]);
            return 1;
        }
        
        Counts counts = count_stream(fp);
        fclose(fp);
        
        print_counts(counts, show_lines, show_words, show_chars, argv[i]);
        
        total.lines += counts.lines;
        total.words += counts.words;
        total.chars += counts.chars;
        num_files++;
    }
    
    // Print total if multiple files
    if (num_files > 1) {
        print_counts(total, show_lines, show_words, show_chars, "total");
    }
    
    return 0;
}