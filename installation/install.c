#include "install.h"


int install(char* pk_path) {

    struct stat pkg_stat;
    if (stat(pk_path, &pkg_stat) != 0) {
        printf("Given package not found!\n");
        return -1;
    }            

    //Check for installation deps
   // if (!check_install_deps())
    //    exit(-1);

    char* meta_dir = concat("/tmp/chsys/", pk_path);
    
    if (stat("/tmp/chsys", &pkg_stat) != 0) {
        mkdir("/tmp/chsys", 0777);
    }   

    //Extract files to temporary location
    mkdir(meta_dir, 0777);

    int pk_extract_status;
    pid_t pk_extract_pid = fork();

    if (pk_extract_pid == 0)
        execl("/bin/tar", "/bin/tar", "xf", pk_path, "-C", meta_dir, NULL);
    wait(&pk_extract_status);

    if (pk_extract_status != 0) {

        printf("Error unpacking archive %s\n", pk_path);
        return pk_extract_status;
    }

    //read metadata
    char* meta_name = concat(meta_dir, "/package");
    FILE* metadata = fopen(meta_name, "r");

    char pk_name[255];
    char pk_ver[255];
    char pk_type[255];
    char tmp[255];

    if (metadata == NULL) {
        printf("Error reading metadata of archive %s\n", pk_path);
        return -1;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, metadata)) != -1) {
        char* key = strtok(line, ":");

        if (strcmp(key, "name") == 0) {
            strcpy(tmp, strtok(NULL, ":"));
            strncpy(pk_name, tmp, strlen(tmp) - 1);  
        }

        else if (strcmp(key, "ver") == 0) {
            strcpy(tmp, strtok(NULL, ":"));
            strncpy(pk_ver, tmp, strlen(tmp) - 1);  
        }

        else if (strcmp(key, "type") == 0) {
            strcpy(tmp, strtok(NULL, ":"));
            strncpy(pk_type, tmp, strlen(tmp) - 1);   
        }   
    }

    //Add user
    char* pk_home = concat("/usr/chsys/pkg/", pk_name);
    int useradd_status; 

    pid_t unpack_pid = fork();
    if (unpack_pid == 0) {
        execl("/sbin/useradd", "/sbin/useradd", 
                               "-G", "install", 
                               "-m", "-d", pk_home,
                               "-s", "/bin/false", pk_name, NULL);
    }
    wait(&useradd_status);
    
    if (useradd_status != 0) {
        printf("Unable to create package user!\n");
        return -1;
    }

    int result = 0;
    if (strcmp(pk_type, "bin") == 0)
        result = install_handler(pk_name, concat(meta_dir, "/package.tar"));
    else
        result = build_handler(pk_name, concat(meta_dir, "/package.tar"));

    
    free(pk_home);
    free(meta_dir);
    free(meta_name);

    //Write to pkdb
    return result;
}

//DONE!
int copy_out_files(char* pk_origin, char* pk_target, bool silent) {

    DIR* pk_from = opendir(pk_origin);
    if (pk_from == NULL)
        return -1;

    struct dirent* pkdir_file;


    while ((pkdir_file = readdir(pk_from)) != NULL) {

        if(pkdir_file->d_type != DT_DIR) {

            char* copy_slash_origin = concat(pk_origin, "/");
            char* copy_slash_target = concat(pk_target, "/");

            char* copy_origin = concat(copy_slash_origin, pkdir_file->d_name);
            char* copy_target = concat(copy_slash_target, pkdir_file->d_name);

            //Remove original and copy new
            remove(copy_target);
            int res = copy_file(copy_origin, copy_target);

            if (res < 0) {
                printf("File copy FAILED!\n");
                exit(res);
            }

            if (!silent)
                printf("%s -> %s\n", copy_origin, copy_target);
        }

        else if(pkdir_file->d_type == DT_DIR &&
                strcmp(pkdir_file->d_name, ".") != 0 &&
                strcmp(pkdir_file->d_name, "..") != 0) {


            struct stat pk_dir;
            if (stat(concat(pk_origin, pkdir_file->d_name), &pk_dir) != 0) {
                mkdir(concat(pk_target, pkdir_file->d_name),
                      S_IRGRP | S_IWGRP | S_IXGRP);
            }

            //Done this way to prevent memory leaks from unfreed concats
            char* path_slash_origin = concat(pk_origin, "/");
            char* path_slash_target = concat(pk_target, "/");

            char* pk_final_origin = concat(path_slash_origin, pkdir_file->d_name);
            char* pk_final_target = concat(path_slash_target, pkdir_file->d_name);

            copy_out_files(pk_final_origin, pk_final_target, silent); // recall with the new path

            free(path_slash_origin);
            free(path_slash_target);
            free(pk_final_origin);
            free(pk_final_target);
        }
    }

    closedir(pk_from); // finally close the directory
    return 0;
}

int worker_build (int uid, const char* pk_path, const char* pk_home) {

    setuid(uid);

    //Copy out all neceseary files to fakeroot
    int pk_install_status;
    pid_t pk_extract_pid = fork();

    if (pk_extract_pid == 0)
        execl("tar", "-xf", pk_path, "-C", pk_home, NULL);
    wait(&pk_install_status);
    
    if (pk_install_status != 0) exit(-1);

    //check if install folder exist
    //if yes, run preinstall script
    struct stat pk_install;
    if (stat(concat(pk_home, "/install/pre.sh"), &pk_install) == 0) {
        int pk_preinstall = fork();
        if (pk_preinstall == 0)
            execl("/bin/bash", "-c", concat(pk_home, "/install/pre.sh"), NULL);
        wait(NULL);
    } 

    if (chdir(pk_home) != 0) {
        printf("Unable to change working directory!\n");
        exit(-1);
    }

    if (stat(concat(pk_home, "/makefile"), &pk_install) != 0) {
        printf("No build script provided!\n");
        exit(-1);
    }

    int pk_make_res = 0;

    int pk_make = fork();
    if (pk_make == 0)
        execl("/bin/make", "all", NULL);
    wait(&pk_make_res); 

    if (pk_make_res != 0) {
        printf("Package build failed!\n");
        exit(-1);
    }

    //run post_install script
    if (stat(concat(pk_home, "/install/post.sh"), &pk_install) == 0) {
        int pk_preinstall = fork();
        if (pk_preinstall == 0)
            execl("/bin/bash", "-c", concat(pk_home, "/install/post.sh"), NULL);
        wait(NULL);
    } 

    exit(0);
}

int worker_install(int uid, const char* pk_path, const char* pk_home) {
    
    setuid(uid);

    

    //Copy out all neceseary files to fakeroot
    int pk_install_status;
    pid_t pk_extract_pid = fork();

    if (pk_extract_pid == 0)
        execl("/bin/tar", "/bin/tar", "xf", pk_path, "-C", pk_home, NULL);
    wait(&pk_install_status);
    
    if (pk_install_status != 0) exit(-1);

    //check if install folder exist
    //if yes, run preinstall script
    struct stat pk_install;
    if (stat(concat(pk_home, "/install/pre.sh"), &pk_install) == 0) {
        int pk_preinstall = fork();
        if (pk_preinstall == 0)
            execl("/bin/bash", "/bin/bash", "-c", concat(pk_home, "/install/pre.sh"), NULL);
        wait(NULL);
    }

    //copy out files
    printf("\nCopying files\n");
    int res = copy_out_files(concat(pk_home, "/package/"), "/", false);
    if (res != 0)
        printf("%n", res);

    //run post_install script
    if (stat(concat(pk_home, "/install/post.sh"), &pk_install) == 0) {
        int pk_preinstall = fork();
        if (pk_preinstall == 0)
            execl("/bin/bash", "/bin/bash", "-c", concat(pk_home, "/install/post.sh"), NULL);
        wait(NULL);
    } 
    exit(0);
}

int build_handler(const char* pk_name, const char* pk_path) {
    //Get user's uid and gid
    struct passwd* p;
    struct group* g;

    if ((p = getpwnam(pk_name)) == NULL) {
        printf("Unable to get uid of package user!\n");
        return -1;
    }
    int pk_uid = p->pw_uid;

    if ((g = getgrnam("install")) == NULL) {
        printf("Unable to get gid of install group!\n");
    }
    int pk_gid = g->gr_gid;

    printf("Switching to package user. UID: %d, GID: %d\n", pk_uid, pk_gid);

    struct stat pk_archive;   
    if (stat(pk_path, &pk_archive) != 0) {
        printf("Unable to locakte package archive!\n");
        return -1;
    }
    chown(pk_path, pk_uid, pk_gid);

    char* pk_home = concat("/usr/chsys/pkg/", pk_name);

    //Fork, so we can safely change user
    pid_t pk_install = fork();
    int pk_install_status = 0;

    if(pk_install == 0) worker_install(pk_uid, pk_path, pk_home);
    wait(&pk_install_status);


    free(pk_home);
    return pk_install_status;
}

int install_handler (const char* pk_name, const char* pk_path) {

    //Get user's uid and gid
    struct passwd* p;
    struct group* g;

    if ((p = getpwnam(pk_name)) == NULL) {
        printf("Unable to get uid of package user!\n");
        return -1;
    }
    int pk_uid = p->pw_uid;

    if ((g = getgrnam("install")) == NULL) {
        printf("Unable to get gid of install group!\n");
    }
    int pk_gid = g->gr_gid;

    printf("Switching to package user. UID: %d, GID: %d\n", pk_uid, pk_gid);


    struct stat pk_archive;   
    if (stat(pk_path, &pk_archive) != 0) {
        printf("Unable to locakte package archive!\n");
        return -1;
    }
    chown(pk_path, pk_uid, pk_gid);

    char* pk_home = concat("/usr/chsys/pkg/", pk_name);

    //Fork, so we can safely change user
    pid_t pk_install = fork();
    int pk_install_status = 0;

    if(pk_install == 0) worker_install(pk_uid, pk_path, pk_home);
    wait(&pk_install_status);
    free(pk_home);
    return pk_install_status;
}

int rem(const char* pk_name) {


    char* pk_origin = concat("/usr/chsys/pkg/", pk_name);

    struct stat pkg_stat;
    if (stat(pk_origin, &pkg_stat) != 0) {
        printf("%s: Package not found!\n", pk_name);
        return -1;
    }

    int pk_rem_status;
    pid_t pk_rem_fork = fork();

    remove_recursive(pk_origin, "/", false);

    if (pk_rem_fork == 0)
        execl("/sbin/userdel", "/sbin/userdel", pk_name, NULL);
    wait(&pk_rem_status);

    remove(pk_origin);
    free(pk_origin);

    //Call userdel
    return 0;
}

int remove_recursive(char* pk_origin, char* pk_target, bool root) {

    DIR* pk_from = opendir(pk_origin);
    if (pk_from == NULL)
        return -1;

    struct dirent* pkdir_file;


    while ((pkdir_file = readdir(pk_from)) != NULL) {

        if(pkdir_file->d_type != DT_DIR) {

            char* copy_slash_origin = concat(pk_origin, "/");
            char* copy_slash_target = concat(pk_target, "/");

            char* copy_origin = concat(copy_slash_origin, pkdir_file->d_name);
            char* copy_target = concat(copy_slash_target, pkdir_file->d_name);

            //Remove original and copy new
            remove(copy_target);
            remove(copy_origin);
            //printf("%s -> %s\n", copy_origin, copy_target);
            if (root)
                printf("Removing: %s\n", copy_target);
            else
                printf("Removing: %s\n", copy_origin);
        }

        else if(pkdir_file->d_type == DT_DIR &&
                strcmp(pkdir_file->d_name, ".") != 0 &&
                strcmp(pkdir_file->d_name, "..") != 0) {

            //Done this way to prevent memory leaks from unfreed concats
            char* path_slash_origin = concat(pk_origin, "/");
            char* path_slash_target = concat(pk_target, "/");

            char* pk_final_origin = concat(path_slash_origin, pkdir_file->d_name);
            char* pk_final_target = concat(path_slash_target, pkdir_file->d_name);

            if (strcmp(pkdir_file->d_name, "package") == 0) {
                remove_recursive(pk_final_origin, "/", true); // recall
            }
            else
                remove_recursive(pk_final_origin, pk_final_target, root);
            remove(pk_final_origin);

            free(path_slash_origin);
            free(path_slash_target);
            free(pk_final_origin);
            free(pk_final_target);
        }
    }
    closedir(pk_from); // finally close the directory
}

int query() {

    char* pk_origin = "/usr/chsys/pkg";

    DIR* pk_from = opendir(pk_origin);
    if (pk_from == NULL)
        return -1;

    struct dirent* pkdir_file;


    while ((pkdir_file = readdir(pk_from)) != NULL) {


        if(pkdir_file->d_type == DT_DIR &&
                strcmp(pkdir_file->d_name, ".") != 0 &&
                strcmp(pkdir_file->d_name, "..") != 0) {

            printf("%s\n", pkdir_file->d_name);
        }
    }

    closedir(pk_from); // finally close the directory
    //Call userdel
    return 0;
}