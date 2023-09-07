#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

void downloadExtract() {
    pid_t pidDownload, pidExtract;
    int status;
    char *downloadArgs[] = {"wget", "-O", "database.zip", "https://drive.google.com/uc?id=1zEAneJ1-0sOgt13R1gL4i1ONWfKAtwBF&export=download", NULL};
    char *unzipArgs[] = {"unzip", "database.zip", NULL};

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
    if (remove("database.zip") != 0) {
        perror("remove error");
    }
}

void deleteNonManUtdFiles() {
    DIR *dir;
    struct dirent *entry;
    char *match = "ManUtd";
    char *filename;
    char filepath[256];

    // Open the directory path "/home/eezr/ngoding/rawr/"
    dir = opendir("/home/eezr/ngoding/rawr/players/");
    if (dir == NULL) {
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    // Traverse the directory entries
    while ((entry = readdir(dir)) != NULL) {
        filename = entry->d_name;
        if (strstr(filename, match) == NULL) {
            // The filename doesn't contain "ManUtd", so delete it
            snprintf(filepath, sizeof(filepath), "/home/eezr/ngoding/rawr/players/%s", filename);
            if (remove(filepath) != 0) {
                perror("remove error");
            } else {
                printf("Deleted file: %s\n", filename);
            }
        }
    }

    // Close the directory
    closedir(dir);
}

void categorizePlayers() {
    DIR *dir;
    struct dirent *entry;
    pid_t pid;
    int status;

    // Open the directory path
    dir = opendir("/home/eezr/ngoding/rawr/players/");
    if (dir == NULL) {
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    // Iterate over each file in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Get the filename
        char *filename = entry->d_name;

        // Determine the position of the player based on their name
        char *position = NULL;
        if (strstr(filename, "Kiper") != NULL) {
            position = "Kiper";
        } else if (strstr(filename, "Bek") != NULL) {
            position = "Bek";
        } else if (strstr(filename, "Gelandang") != NULL) {
            position = "Gelandang";
        } else if (strstr(filename, "Penyerang") != NULL) {
            position = "Penyerang";
        } else {
            // Skip files that are not related to ManUtd players
            continue;
        }

        // Create the directory for the player's position if it doesn't exist
        pid = fork();
        if (pid == 0) {
            char folderpath[256];
            snprintf(folderpath, sizeof(folderpath), "/home/eezr/ngoding/rawr/players/%s", position);
            char *newFolder[] = {"mkdir", "-p", folderpath, NULL};
            execvp(newFolder[0], newFolder);
            perror("mkdir error");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, &status, 0);
        }

        // Move the player's file to the appropriate position folder
        pid = fork();
        if (pid == 0) {
            char oldpath[256], newpath[256];
            snprintf(oldpath, sizeof(oldpath), "/home/eezr/ngoding/rawr/players/%s", filename);
            snprintf(newpath, sizeof(newpath), "/home/eezr/ngoding/rawr/players/%s/%s", position, filename);
            char *mvArgs[] = {"mv", oldpath, newpath, NULL};
            execvp(mvArgs[0], mvArgs);
            perror("mv error");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, &status, 0);
        }
    }

    // Close the directory
    closedir(dir);
}

int main() {
    downloadExtract();
    deleteNonManUtdFiles();
    categorizePlayers();
    return 0;
}