#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<limits.h>

#define MAGIC "0T"
#define MIN_VERSION 47
#define MAX_VERSION 130
#define MIN_SECTIONS 6
#define MAX_SECTIONS 16



// typedef struct {
//     char name[19];
//     char type;
//     int offset;
//     int size;
// } section_header;

// typedef struct {
//     char magic[3];
//     short header_size;
//     char version;
//     char no_of_sections;
//     section_header *section_headers;
// } sf_header;

// int is_valid_sf(sf_header *header) {
//     // verific magic
//     if (strncmp(header->magic, MAGIC, 2) != 0) {
//         printf("ERROR\nwrong magic\n");
//         return 0;
//     }

//     // verific versiunea
//     if (header->version < MIN_VERSION || header->version > MAX_VERSION) {
//         printf("ERROR\nwrong version\n");
//         return 0;
//     }

//     // verific nr sectiunilor
//     if (header->no_of_sections < MIN_SECTIONS || header->no_of_sections > MAX_SECTIONS) {
//         printf("ERROR\nwrong sect_nr\n");
//         return 0;
//     }

//     //verific tipul sectiunii
//     char valid_types[] = {76, 27, 77, 79, 93};
//     int i, j, valid;
//     for (i = 0; i < header->no_of_sections; i++) {
//         valid = 0;
//         for (j = 0; j < 5; j++) {
//             if (header->section_headers[i].type == valid_types[j]) {
//                 valid = 1;
//                 break;
//             }
//         }
//         if (!valid) {
//             printf("ERROR\nwrong sect_types\n");
//             return 0;
//         }
//     }

//     return 1;
// }

int parse_sf_file(const char *file_path) {
    
    int variabila=0;
    int fd= open(file_path, O_RDONLY);
    if (fd < 0) {
      printf("ERROR\nfile not found\n");
        return -1;
    }

  lseek(fd, 0, SEEK_SET);
    unsigned char magic1;
    unsigned char magic2;

    read (fd, &magic1, 1);
    read (fd, &magic2, 1);
      if(magic1 != 48 && magic2!= 84)
    {
        printf("ERROR\nwrong magic\n");
        close(fd);
        return -1;
    }

    unsigned short header_size1;
    read(fd, &header_size1, 2);
    

    unsigned short version1;
    read(fd, &version1, 2);
   
  if( version1< 47 || version1 > 130)
    {
        printf("ERROR\nwrong version\n");
        close(fd);
        return -1;
    }

    unsigned short no_of_sections1;
    read(fd, &no_of_sections1, 1);

     if( no_of_sections1 < 6 || no_of_sections1 > 16)
    {
        printf("ERROR\nwrong sect_nr\n");
        close(fd);
        return -1;
    }


     

    
    for(int i=0;i<no_of_sections1;i++)
    {
        char section_name[19];
        read(fd, &section_name, 18);
        section_name[18]= '\0';
        unsigned char section_type;
        read(fd, &section_type, 1);
        unsigned int section_offset;
        read(fd, &section_offset, 4);
        unsigned int section_size;
        read(fd, &section_size, 4);
  if (section_type != 76 && section_type != 27 && section_type != 77 && section_type != 79 && section_type != 93 ) {
            variabila=1;
        }
        
    }

    if(variabila==1)  {
        printf("ERROR\nwrong sect_types\n");
        close(fd);
        return -1;
    }


    if(variabila==0)
    {
        int fd= open(file_path, O_RDONLY);
    if (fd < 0) {
        printf("ERROR\nfile not found\n");
        return -1;
    } 

 lseek(fd, 0, SEEK_SET);
    // unsigned char magic1;
    // unsigned char magic2;

    read (fd, &magic1, 1);
    read (fd, &magic2, 1);
 

    // unsigned short header_size1;
    read(fd, &header_size1, 2);

    // unsigned short version1;
    read(fd, &version1, 2);
  
    // unsigned short no_of_sections1;
    read(fd, &no_of_sections1, 1);

    printf("SUCCESS\nversion=%d\nnr_sections=%d\n",version1, no_of_sections1);

    for(int i=0;i<no_of_sections1;i++)
    {
        char section_name[19];
        read(fd, &section_name, 18);
        section_name[18]= '\0';
        unsigned char section_type;
        read(fd, &section_type, 1);
        unsigned int section_offset;
        read(fd, &section_offset, 4);
        unsigned int section_size;
        read(fd, &section_size, 4);

        printf("section%d: %s %d %d\n",i+1,section_name,section_type,section_size);
    }
    }
   

    close(fd);

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
        char full_path[2048];
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
                printf("SUCCESS\n");
                if (list_dir(path, recursive, name_starts_with, has_perm_execute) == 0) {
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
