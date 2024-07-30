#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Case-insensitive string search
char *my_strcasestr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    for (; *haystack; ++haystack) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h, *n;
            for (h = haystack, n = needle; *h && *n; ++h, ++n) {
                if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) break;
            }
            if (!*n) return (char *)haystack;
        }
    }
    return NULL;
}

// Searches a line for a pattern with various options
bool search_line(const char *line, const char *pattern, bool case_insensitive, bool whole_word, bool whole_line) {
    const char *start = line;
    while ((start = case_insensitive ? my_strcasestr(start, pattern) : strstr(start, pattern)) != NULL) {
        size_t pattern_len = strlen(pattern);
        
        if (whole_word) {
            bool word_start = (start == line) || !isalnum(*(start - 1));
            bool word_end = !isalnum(*(start + pattern_len));
            if (word_start && word_end) {
                return true;
            }
        } else if (whole_line) {
            if (strcmp(start, pattern) == 0) {
                return true;
            }
        } else {
            return true;
        }
        start += pattern_len;
    }
    return false;
}

// Checks if a file is binary by scanning the first 512 bytes
bool is_binary_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        return false;
    }
    unsigned char buffer[512];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);
    
    for (size_t i = 0; i < bytes_read; i++) {
        if (buffer[i] == 0) {
            return true;
        }
    }
    return false;
}

// Processes a single file with given options
void grep_file(const char *filename, const char *pattern, bool case_insensitive, bool line_numbers, bool invert_match, 
               bool count_matches, bool show_filenames, bool whole_word, bool whole_line, bool byte_offset, int context,
               const char *binary_files) {
    if (is_binary_file(filename)) {
        if (strcmp(binary_files, "skip") == 0) {
            return; // Skip binary files
        } else if (strcmp(binary_files, "binary") == 0) {
            printf("Binary file %s matches\n", filename);
            return;
        }
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char line[1024];
    int line_number = 1;
    int match_count = 0;
    long offset = 0;

    while (fgets(line, sizeof(line), file)) {
        size_t line_len = strlen(line); // Calculate line length once
        bool match = search_line(line, pattern, case_insensitive, whole_word, whole_line);
        if (invert_match) {
            match = !match;
        }

        if (match) {
            match_count++;
            if (show_filenames) {
                printf("%s:", filename);
            }
            if (line_numbers) {
                printf("%d:", line_number);
            }
            if (byte_offset) {
                printf("%ld:", offset);
            }
            printf("%s", line);
            if (context > 0) {
                // Show context lines
                long cur_pos = ftell(file);
                for (int i = 0; i < context; i++) {
                    if (fgets(line, sizeof(line), file)) {
                        printf("%s", line);
                    } else {
                        break;
                    }
                }
                fseek(file, cur_pos, SEEK_SET); // Restore file position
            }
        }
        offset += line_len; // Update offset with pre-calculated line length
        line_number++;
    }

    if (count_matches) {
        printf("%s: %d matches\n", filename, match_count);
    }

    fclose(file);
}

// Recursively searches through a directory
void grep_directory(const char *dirname, const char *pattern, bool case_insensitive, bool line_numbers, bool invert_match, 
                    bool count_matches, bool show_filenames, bool whole_word, bool whole_line, bool byte_offset, int context,
                    const char *binary_files) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip '.' and '..'
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
            struct stat path_stat;
            if (stat(path, &path_stat) != 0) {
                perror("stat");
                continue; // Skip file if stat fails
            }
            if (S_ISDIR(path_stat.st_mode)) {
                grep_directory(path, pattern, case_insensitive, line_numbers, invert_match, count_matches, show_filenames, whole_word, whole_line, byte_offset, context, binary_files);
            } else {
                grep_file(path, pattern, case_insensitive, line_numbers, invert_match, count_matches, show_filenames, whole_word, whole_line, byte_offset, context, binary_files);
            }
        }
    }

    closedir(dir);
}
