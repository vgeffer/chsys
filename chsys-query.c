#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

    if (geteuid() != 0) {
        fprintf(stderr, "To modify your system, chsys needs to run as root.\n");
        return 1;
    }

    FILE* pkdb;
    struct stat db;
    if (stat("/etc/chsysdb", &db) != 0) {
        printf("No packages installed\n");
        return 0;
    }   
    else pkdb = fopen("/etc/chsysdb", "r");

    char * line = NULL;
    size_t len = 0;
    size_t read;

    while ((read = getline(&line, &len, pkdb)) != -1) {
        printf("%s\n", line);
    }

    fclose(pkdb);
}