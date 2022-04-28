#include "util.h"

char* concat(char* a,  char* b) {
    char* result = malloc(sizeof(char) * (strlen(a) + strlen(b)) + 1);
    sprintf(result, "%s%s\00", a, b); //adds safety null terminator
    return result;
}

char** split(char* a, char delim) {

    uint32_t substr_count = 0;
    char **substrings, *string;

    string = malloc(sizeof(char) * strlen(a) + 1); //for a safety null byte at the end
    memcpy(string, a, strlen(a) * sizeof(char));
    *(string + sizeof(char) * strlen(a)) = '\00';


    //Get the count of substrings
    while (*a) {
        if (*a == delim) substr_count++;
        a += 1;
    }

    substrings = malloc(sizeof(char*) * substr_count);
    *substrings = string;

    //Get the addresses of substring
    while (*string) {
        if (*string == delim) { 
            *string = '\00';
            *substrings = string + 1;
        } 

        substrings += 1;
        string += 1; 
    }

    return substrings;
}

int copy_file(char* src, char* dest) {
    int in = open(src, O_RDONLY);
    int out = open(dest, O_CREAT | O_WRONLY, 0770);
   
    if (in < 0 || out < 0) {

        if (in == -1 && errno == EACCES)
            printf("EACCES in %d\n", in);
        if (out == -1 && errno == EACCES)
            printf("EACCES out %d\n", in);
        return -1;


    }


    char buf[8192];
    posix_fadvise(in, 0, 0, POSIX_FADV_SEQUENTIAL);


    while (1) {
        size_t read_result = read(in, &buf[0], sizeof(buf));
        if (!read_result) break; //If there is nothing more to read, stop
    
        size_t write_result = write(out, &buf[0], read_result);
        if(write_result != read_result) {
            close(in);
            close(out);
            remove(dest); //If there is a problem writing, delete anything thet has bee already written
            return -1;
        }
    }

    close(in);
    close(out);
    return 0;
}
Mutex* create_mutex(char* lock_file) {

    int lock_fd = open(lock_file, O_CREAT | O_RDWR, 0);

    if (lock_fd < 0)
        return NULL;
 

    Mutex* out = malloc(sizeof(Mutex));
    out->lock = lock_fd;
    return out;
}

int destroy_mutex(Mutex* m) {

    if (!m)
        return -1;

    close(m->lock);
    free(m);
    return 0;
}

int lock(Mutex* m) {
    return flock(m->lock, LOCK_EX);
}

int try_lock(Mutex* m) {
    return flock(m->lock, LOCK_EX | LOCK_NB);
}

int unlock(Mutex* m) {
    return flock(m->lock, LOCK_UN);
}