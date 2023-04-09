#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define MAGIC "0T"
#define MIN_VERSION 47
#define MAX_VERSION 130
#define MIN_SECTIONS 6
#define MAX_SECTIONS 16



typedef struct {
    char name[19];
    char type;
    int offset;
    int size;
} section_header;

typedef struct {
    char magic[3];
    short header_size;
    char version;
    char no_of_sections;
    section_header *section_headers;
} sf_header;

int is_valid_sf(sf_header *header) {
    // verific magic
    if (strncmp(header->magic, MAGIC, 2) != 0) {
        printf("ERROR\nwrong magic\n");
        return 0;
    }

    // verific versiunea
    if (header->version < MIN_VERSION || header->version > MAX_VERSION) {
        printf("ERROR\nwrong version\n");
        return 0;
    }

    // verific nr sectiunilor
    if (header->no_of_sections < MIN_SECTIONS || header->no_of_sections > MAX_SECTIONS) {
        printf("ERROR\nwrong sect_nr\n");
        return 0;
    }

    //verific tipul sectiunii
    char valid_types[] = {76, 27, 77, 79, 93};
    int i, j, valid;
    for (i = 0; i < header->no_of_sections; i++) {
        valid = 0;
        for (j = 0; j < 5; j++) {
            if (header->section_headers[i].type == valid_types[j]) {
                valid = 1;
                break;
            }
        }
        if (!valid) {
            printf("ERROR\nwrong sect_types\n");
            return 0;
        }
    }

    return 1;
}

int parse_sf_file(char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("ERROR\nfile not found\n");
        return -1;
    }

    // citesc header
    sf_header header;
    fread(&header, sizeof(sf_header), 1, file);


    //verific daca fisierul e sf valid
    if (!is_valid_sf(&header)) {
        fclose(file);
        return -1;
    }


    printf("SUCCESS\n");
    printf("version=%d\n", header.version);
    printf("nr_sections=%d\n", header.no_of_sections);

    // afisez section headers
    int i;
    for (i = 0; i < header.no_of_sections; i++) {
        printf("section%d: %s %d %d\n", i+1, header.section_headers[i].name, 
               header.section_headers[i].type, header.section_headers[i].size);
    }

    fclose(file);
    return 0;
}


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

char *file_path = NULL;

    // parse command line arguments
    int i;
    for (i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (strncmp(arg, "path=", 5) == 0) {
            file_path = arg + 5;
        }
    }

    if (file_path == NULL) {
        printf("ERROR\nfile path not specified\n");
        return -1;
    }

    // parse SF file
    return parse_sf_file(file_path);
}

