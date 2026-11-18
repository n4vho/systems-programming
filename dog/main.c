#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define PURPLE "\033[38;2;187;154;247m"
#define RESET  "\033[0m"
#define BOLD "\033[1m"

void dog(const char *path) {
    // i dont want to show the huge abs path
    const char *filename = strrchr(path, '/');
    filename = filename ? filename+1 : path;


    FILE *fp = fopen(path, "rb"); // rb = r for textfiles, b for binary mode
    if (!fp) {
        perror("No such file or directory");
        return;
    }
    char buffer[4096];
    size_t bytes_read;
    int is_first_chunk = 1;

    while((bytes_read = fread(buffer, 1, 4096, fp)))
    {
        // checking fist 4096 bytes for null terminator
        // goal is to ignore bin files
        if (is_first_chunk) {
            for (size_t i = 0; i < bytes_read; i++){
                if(buffer[i] == '\0') {
                    fclose(fp);
                    return;
                }                    
            }
            is_first_chunk = 0;
        }

        printf(BOLD "\n%s============= %s ==============%s\n\n", PURPLE, filename, RESET);
        fwrite(buffer, 1, bytes_read, stdout);
    }

    if (ferror(stdout)) {
        fprintf(stderr, "Error writing to stdout\n");
        return;
    }

    printf("\n");
    fclose(fp);

}

void walk_dog (const char *path) {

    struct dirent *fp;
    const char *target;
    char full_path[4096];

    DIR *dir = opendir(path);

    if (!dir) {
        perror ("Unable to read directory.");
        return;
    }

    while(fp = readdir(dir)) {
        if (strcmp(fp->d_name, ".") == 0 ||
            strcmp(fp->d_name, "..") == 0) continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, fp->d_name);

        if(fp->d_type == DT_DIR)   //if dir -> walk the dir-path
            walk_dog(full_path);
        else dog (full_path);      // else just print
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    // if no filename show command usaeg
    if (argc == 1) {
        printf("Usage:> %s <file_path>...\n", argv[0]);
    }

    // loop thru every file
    for (int i = 1; i < argc; i++) {
        const char* path = argv[i];
        struct stat statbuf;
        
        //check if file is a directory
        if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
        {
            walk_dog(path);
        }
        
        else dog(path);
    }

    return 0;
}
