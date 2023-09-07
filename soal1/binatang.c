#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>

void downloadAndUnzip() {
    pid_t pidDownload, pidExtract;
    int status;
    char *downloadArgs[] = {"wget", "-O", "binatang.zip", "https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq", NULL};
    char *unzipArgs[] = {"unzip", "binatang.zip", NULL};

    // Fork a child process to run the download command
    pidDownload = fork();
    if (pidDownload == 0) {
        execvp(downloadArgs[0], downloadArgs);
        perror("download error");
        exit(EXIT_FAILURE);
    } else if (pidDownload < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    // Wait for the download process to finish
    waitpid(pidDownload, &status, 0);

    // Fork another child process to run the extract command
    pidExtract = fork();
    if (pidExtract == 0) {
        execvp(unzipArgs[0], unzipArgs);
        perror("extract error");
        exit(EXIT_FAILURE);
    } else if (pidExtract < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    // Wait for the extract process to finish
    waitpid(pidExtract, &status, 0);

    // Remove the zip file
    if (remove("binatang.zip") != 0) {
        perror("remove error");
    }
}

void randomPicture() {
    DIR *dir;
    struct dirent *ent;
    int pictureNum = 0;
    char **picturePath;
    char *ext;
    int i, index;
    time_t t;

    // randomize using time
    srand((unsigned) time(&t));

    // open the current directory for reading
    dir = opendir(".");
    if (dir == NULL) {
        perror("ERROR: opendir");
        exit(EXIT_FAILURE);
    }

    // read each directory entry
    while ((ent = readdir(dir)) != NULL) {
        ext = strrchr(ent->d_name, '.');
        if (ext != NULL && (strcmp(ext, ".jpg") == 0)) {
            pictureNum++;
        }
    }

    // check if no picture files were found
    if (pictureNum == 0) {
        printf("No pictures found.\n");
        return;
    }

    // allocate memory for the array of picture file paths
    picturePath = (char**) malloc(pictureNum * sizeof(char*));
    if (picturePath == NULL) {
        perror("ERROR: malloc");
        exit(EXIT_FAILURE);
    }

    // reset the directory stream to the beginning
    rewinddir(dir);

    // read each directory entry again
    i = 0;
    while ((ent = readdir(dir)) != NULL) {
        ext = strrchr(ent->d_name, '.');
        if (ext != NULL && (strcmp(ext, ".jpg") == 0)) {
            picturePath[i] = ent->d_name;
            i++;
        }
    }

    index = rand() % pictureNum;

    printf("Next Shift: %s\n", picturePath[index]);

    // free the memory
    closedir(dir);
    free(picturePath);
}

void filterFile() {
    // Open the directory containing the unzipped pictures
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    // Create the directories if they don't exist
    if (access("hewanDarat", F_OK) == -1) {
        mkdir("hewanDarat", 0777);
    }
    if (access("hewanAir", F_OK) == -1) {
        mkdir("hewanAir", 0777);
    }
    if (access("hewanAmphibi", F_OK) == -1) {
        mkdir("hewanAmphibi", 0777);
    }

    // Iterate through the files in the directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            // Check if the file name contains "darat", "air", or "amphibi"
            char *filename = entry->d_name;
            if (strstr(filename, "darat") != NULL) {
                // If the file name contains "darat", move it to the "hewanDarat" directory
                char *args[] = {"mv", filename, "./hewanDarat", NULL};
                pid_t pid = fork();
                                    
                if(pid == 0){
                    execvp("mv", args);
                }
                                    
            } else if (strstr(filename, "air") != NULL) {
                // If the file name contains "air", move it to the "hewanAir" directory
                char *args[] = {"mv", filename, "./hewanAir", NULL};
                pid_t pid = fork();
                                    
                if(pid == 0){
                    execvp("mv", args);
                }

            } else if (strstr(filename, "amphibi") != NULL) {
                // If the file name contains "amphibi", move it to the "hewanAmphibi" directory
                char *args[] = {"mv", filename, "./hewanAmphibi", NULL};
                pid_t pid = fork();
                                    
                if(pid == 0){
                    execvp("mv", args);
                }

            }
        }
    }

    // Close the directory
    closedir(dir);
}

void zipDirectory(char *directory) {
    pid_t pid;
    int status;

    // Construct the command to zip the directory
    char *zipArgs[] = {"zip", "-r", "-q", "-9", directory, directory, NULL};

    // Fork a child process to run the zip command
    pid = fork();
    if (pid == 0) {
        execvp(zipArgs[0], zipArgs);
        perror("zip error");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    // Wait for the zip process to finish
    waitpid(pid, &status, 0);

    // Remove the directory
    char *rmArgs[] = {"rm", "-rf", directory, NULL};
    pid = fork();
    if (pid == 0) {
        execvp(rmArgs[0], rmArgs);
        perror("rm error");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    // Wait for the rm process to finish
    waitpid(pid, &status, 0);
}

int main() {
    downloadAndUnzip();
    randomPicture();
    filterFile();

    // Zipping
    zipDirectory("hewanDarat");
    zipDirectory("hewanAir");
    zipDirectory("hewanAmphibi");
    return 0;
}
