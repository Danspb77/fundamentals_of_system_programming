#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void search_in_file(const char *filename, const char *search_sequence) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file: %s\n", filename);
        return;
    }

    size_t search_len = strlen(search_sequence) - 2;
    unsigned char *search_bytes = (unsigned char *)malloc((search_len / 2) * sizeof(unsigned char));
    if (search_bytes == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return;
    }

    sscanf(search_sequence + 2, "%2hhx", &search_bytes[0]);
    for (size_t i = 1; i < search_len / 2; i++) {
        sscanf(search_sequence + 2 * i + 2, "%2hhx", &search_bytes[i]);
    }

    unsigned char buffer[1024];
    size_t bytes_read;
    size_t match_index = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == search_bytes[match_index]) {
                match_index++;
                if (match_index == search_len / 2) {
                    if (getenv("LAB11DEBUG") != NULL) {
                        fprintf(stderr, "Found sequence in file: %s (position: %ld)\n", filename, ftell(file) - search_len / 2 + 1);
                    }
                    printf("Found sequence in file: %s\n", filename);
                    fclose(file);
                    free(search_bytes);
                    return;
                }
            } else {
                match_index = (buffer[i] == search_bytes[0]) ? 1 : 0;
            }
        }
    }

    fclose(file);
    free(search_bytes);
}

void search_in_directory(const char *path, const char *search_sequence) {
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
            search_in_directory(full_path, search_sequence);
        } else {
            search_in_file(full_path, search_sequence);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <search_sequence>\n", argv[0]);
        return 1;
    }

    const char *directory = argv[1];
    const char *search_sequence = argv[2];

    search_in_directory(directory, search_sequence);

    return 0;
}
