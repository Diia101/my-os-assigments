#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256

typedef struct {
    char magic[3];
    int version;
    int nr_sections;
} Header;

typedef struct {
    int type;
    int offset;
    int size;
} Section;

int is_sf_file_valid(const char* file_path, Header* header, Section** sections, int* num_sections);
void print_error(const char* message);
void print_success(const Header* header, const Section* sections, int num_sections);


int list_dir(char *path, int recursive, char *name_starts_with, int has_perm_execute) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("ERROR\n invalid directory path\n");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (entry->d_type == DT_DIR && recursive) {
            if (list_dir(full_path, recursive, name_starts_with, has_perm_execute) != 0) {
                closedir(dir);
                return 1;
            }
        }
        if (name_starts_with != NULL && strncmp(entry->d_name, name_starts_with, strlen(name_starts_with)) != 0) {
            continue;
        }
        if (has_perm_execute && access(full_path, X_OK) != 0) {
            continue;
        }
        printf("%s\n", full_path);
    }
    closedir(dir);
    return 0;
}


int main(int argc, char **argv){
    
    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("61586\n");
            return 0;
        }

        if(strcmp(argv[1], "list") == 0){
            char *path = NULL;
            int recursive = 0;
            char *name_starts_with = NULL;
            int has_perm_execute = 0;
 

            for (int i = 2; i < argc; i++) {
                 if (strncmp(argv[i], "path=", 5) == 0) {
                    path = argv[i] + 5;
                } else if (strcmp(argv[i], "recursive") == 0) {
                    recursive = 1;
                } else if (strncmp(argv[i], "name_starts_with=", 17) == 0) {
                    name_starts_with = argv[i] + 17;
                } else if (strcmp(argv[i], "has_perm_execute") == 0) {
                    has_perm_execute = 1;
                }
            }

            if (path != NULL) {
                if (list_dir(path, recursive, name_starts_with, has_perm_execute) == 0) {
                    printf("SUCCESS\n");
                    return 0;
                } else {
                    return 1;
                }
            } else {
                printf("ERROR\n missing directory path\n");
                return 1;
            }
        }
    }

    printf("ERROR\n invalid arguments\n");

    if (argc < 3) {
        printf("Usage: %s parse path=<file_path>\n", argv[0]);
        return 1;
    }
}

