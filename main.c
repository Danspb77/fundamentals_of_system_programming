#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void search_in_file(const char *filename, const char *search_bytes) {
    FILE *file = fopen(filename, "rb"); // Open the file in binary mode
    if (file == NULL) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return;
    }

    unsigned char buffer[1024]; // Buffer for reading bytes from the file
    size_t search_len = strlen(search_bytes) / 2; // Length of search bytes
    unsigned char *search_buffer = (unsigned char *)malloc(search_len); // Buffer to store search bytes in binary format

    // Convert the search string from hexadecimal to binary
    for (size_t i = 0; i < search_len; i++) {
        sscanf(search_bytes + 2 * i, "%2hhx", &search_buffer[i]);
    }

    size_t bytes_read;
    size_t match_index = 0; // Index to track match progress
    unsigned char byte;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) { // Read file in chunks
        for (size_t i = 0; i < bytes_read; i++) {
            byte = buffer[i];
            if (byte == search_buffer[match_index]) {
                match_index++;
                if (match_index == search_len) {
                    printf("Found sequence in file: %s\n", filename);
                    fclose(file);
                    free(search_buffer);
                    return;
                }
            } else {
                match_index = (byte == search_buffer[0]) ? 1 : 0;
            }
        }
    }

    fclose(file);
    free(search_buffer);
}


void search_in_directory(const char *path, const char *search_str) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if ((dir = opendir(path)) == NULL) {
        fprintf(stderr, "Could not open directory: %s\n", path);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(full_path, &statbuf) == -1) {
            fprintf(stderr, "Failed to get file status: %s\n", full_path);
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            search_in_directory(full_path, search_str);
        } else {
            search_in_file(full_path, search_str);
        }
    }


    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 2 && ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0))) {
        printf("You can use -v to see version and -h to see help\n");
        return 0;
    } else if (argc == 2 && ((strcmp(argv[1], "-v") == 0) || (strcmp(argv[1], "--version") == 0))) {
        printf("Version 1.0\n");
        return 0;
    } else if (argc != 3) {
        printf("Usage: %s <directory> <search_string>\n", argv[0]);
        return 1;
    }

    const char *directory = argv[1];
    const char *search_str = argv[2];

    search_in_directory(directory, search_str);

    return 0;
 }