#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void dog(const char *path) {

    // i dont want to show the huge abs path
    const char *filename = strrchr(path, '/');
    filename = filename ? filename+1 : path;


    printf("\n============= %s ==============\n\n", path);

    FILE *fp = fopen(path, "rb"); // rb = r for textfiles, b for binary mode
    if (!fp) {
        perror("No such file or directory");
        return;
    }

    char buffer[4096];
    size_t bytes_read;

    while((bytes_read = fread(buffer, 1, 4096, fp)))
    {
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

        if(fp->d_type == DT_DIR) walk_dog(full_path);
        else dog (full_path);
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

        if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
        {
            printf("%s\n", path);
            walk_dog(path);
        }

        dog(path);

    }

    return 0;
}
