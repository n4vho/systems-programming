#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>

#define PURPLE "\033[38;2;187;154;247m"
#define RESET  "\033[0m"
#define BOLD "\033[1m"



void print_name (char *filename, bool isDir, bool isLast, int subdir) {
    if(isLast) {
        for(int i = 0; i < subdir; i++)
            printf("|   ");
        if (isDir) printf ("L__ %s%s%s\n", PURPLE, filename, RESET);
        else printf("L__ %s\n", filename);
    }
    else
{
        for(int i = 0; i < subdir; i++)
            printf("|   ");
        if (isDir) printf ("|-- %s%s%s\n", PURPLE, filename, RESET);
        else printf("|-- %s\n", filename);
    }
}

void peek(const char *filename, int subdir)
{
    DIR *dir;
    const char *path = filename;
    struct dirent *ep; // entry point
    struct stat buf;
    char currName[4096];
    bool hasCurr = false;
    bool isDir = false;


    dir = opendir(filename);

    if (dir == NULL){
        perror ("Unable to read directory");
        return;
    }

    while (ep = readdir(dir)) 
    {
        if (strcmp(ep->d_name, ".") == 0 || 
            strcmp(ep->d_name, "..") == 0) continue;

        if(hasCurr) {
            print_name(currName, isDir, false, subdir);

            if(isDir){
                char next_path [4096];
                snprintf(next_path, sizeof(next_path), "%s/%s", path, currName);
                peek(next_path, 1+subdir);
            }
        }

        strncpy(currName, ep->d_name, sizeof(currName) - 1);
        isDir = (ep->d_type == DT_DIR);
        hasCurr = true;
    }

    if(hasCurr)
    {
        print_name(currName, isDir, true, subdir);

        if(isDir){
            char next_path [4096];
            snprintf(next_path, sizeof(next_path), "%s/%s", path, currName);
            peek(next_path, subdir+1);
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{
    char cwd[4096];
    if(argv[1] == NULL) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf(BOLD "\nCurrent working dir: \n\n%s\n" RESET, cwd);
        } 
        else 
        {
            perror("getcwd() error");
            return 1;
        }


        peek(".", 0);
    }
    else
    {
        printf(BOLD "\n\n%s\n" RESET, argv[1]);

        peek (argv[1], 0);
    }
}
