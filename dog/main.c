#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // if no filename show command usaeg
    if (argc == 1) {
        printf("Usage:> %s <file_path>...\n", argv[0]);
    }

    // loop thru every file
    for (int i = 1; i < argc; i++) {
        const char* path = argv[i];

        FILE *fp = fopen(argv[i], "r"); // rb = r for textfiles, b for binary mode
        if (!fp) {
            perror("No such file or directory");
            continue;
        }

        // char buffer[4096];
        // size_t bytes_read;
        //
        // while(bytes_read = fread(buffer, 1, 4096, fp))
        // {
        //     fwrite()
        // }

        char c;

        while( (c = fgetc(fp)) != EOF)
            putchar(c);

        printf("\n");
        
        fclose(fp);
    }

    return 0;
}
