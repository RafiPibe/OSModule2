#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <zlib.h>
#include <dirent.h>
#include <zip.h>

#define FOLDER_NAME_FORMAT "%Y-%m-%d_%H:%M:%S"
#define FILE_NAME_FORMAT   "%Y-%m-%d_%H:%M:%S.jpg"
#define ZIP_FILE_NAME      "images.zip"

void create_folder(const char* folder_name) {
    //time_t current_time = time(NULL);
    //struct tm* time_info = localtime(&current_time);
    //char folder_name[100];
    //strftime(folder_name, sizeof(folder_name), FOLDER_NAME_FORMAT, time_info);
    mkdir(folder_name, 0755);
    chdir(folder_name);

}

//void Image_option(const char* url, const char* file_name) {
  //  pid_t pid = fork();

    //if (pid == -1) {
      //  perror("Tidak bisa di fork");
        //exit(EXIT_FAILURE);
    //} else if (pid == 0) {
      //  char wget_command[256];
       // char convert_command[256];
       // char size_option[20];
       // time_t current_time = time(NULL);

        // Create wget command to download the image from URL
       // sprintf(wget_command, "/usr/bin/wget -q -O %s %s", file_name, url);

        // Create convert command to resize the image
       // sprintf(size_option, "%dx%d", (int)(current_time % 1000) + 50, (int)(current_time % 1000) + 50);
       // sprintf(convert_command, "/usr/bin/convert -resize %s %s %s", size_option, file_name, file_name);

        // Execute the wget command and check for errors
       // if (system(wget_command) == -1) {
         //   perror("Tidak bisa menjalankan perintah wget");
           // exit(EXIT_FAILURE);
        //}

        // Execute the convert command and check for errors
        //if (system(convert_command) == -1) {
          //  perror("Tidak bisa menjalankan perintah convert");
            //exit(EXIT_FAILURE);
        //}

    //    exit(EXIT_SUCCESS);
    //}
//}

void download_IMAGES() {
    for (int image_count = 0; image_count < 15; image_count++) {
	time_t current_time;
        struct tm* time_info;
        char file_name[80];
        char url[50];

        time(&current_time);
        time_info = localtime(&current_time);
        strftime(file_name, sizeof(file_name), FILE_NAME_FORMAT, time_info);
        sprintf(url, "https://picsum.photos/%d", (image_count % 10) + 400);

        FILE* file = fopen(file_name, "wb");
        if (file == NULL) {
            perror("Failed to create file");
            exit(EXIT_FAILURE);
        }

        printf("Downloading %s...\n", file_name);
        fflush(stdout);

        pid_t pid = fork();
        if (pid < 0) {
            perror("Failed to fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process
	    //execl("/usr/bin/wget", "wget", "-q", "-O", file_name, url, NULL);
            char size_option[20];
            sprintf(size_option, "%dx%d", (int)(current_time % 1000) + 50, (int)(current_time % 1000) + 50);
            execl("/usr/bin/wget", "wget", "-q", "-O", file_name, url, NULL);
            execl("/usr/bin/convert", "convert", "-resize", size_option, file_name, file_name, NULL);
        } else {
            // Parent process
            fclose(file);
            sleep(5);
        }
    }
    while (wait(NULL) > 0) {}
}

void generate_killer() {
    char kill_prog[] = "lukisan2.exe";
    FILE* script_file = fopen("killer.sh", "w");
    if (script_file == NULL) {
        perror("Failed to create stop script file");
        exit(EXIT_FAILURE);
    }

    fprintf(script_file, "#!/bin/sh\n");
    fprintf(script_file, "kill $(pgrep -f %s)\n", kill_prog);
    fprintf(script_file, "rm -f stop_daemon.sh\n");

    if (fclose(script_file) != 0) {
        perror("Failed to close stop script file");
        exit(EXIT_FAILURE);
    }

    if (chmod("killer.sh", 0700) != 0) {
        perror("Failed to set stop script file permissions");
        exit(EXIT_FAILURE);
    }

    printf("Kill script generated... \n");
}

void create_zip(const char* folder_name) {
    char zip_name[100];
    sprintf(zip_name, "%s.zip", folder_name);

    char zip_command[256];
    sprintf(zip_command, "/usr/bin/zip -r %s %s", zip_name, folder_name);

    if (system(zip_command) == -1) {
        perror("Tidak bisa menjalankan perintah zip");
        exit(EXIT_FAILURE);
    }
}

void remove_folder(const char* folder_name) {
    DIR* dir_ptr = opendir(folder_name);
    struct dirent* entry;
    struct stat info;

    while ((entry = readdir(dir_ptr)) != NULL) {
        char path[PATH_MAX];
        snprintf(path, PATH_MAX, "%s/%s", folder_name, entry->d_name);

        if (stat(path, &info) == -1) {
            perror("Failed to get file info");
            exit(EXIT_FAILURE);
        }

        if (S_ISDIR(info.st_mode)) {
            // Skip directories "." and ".."
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }

            remove_folder(path);
        } else {
            if (unlink(path) == -1) {
                perror("Failed to remove file");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (closedir(dir_ptr) == -1) {
        perror("Failed to close directory");
        exit(EXIT_FAILURE);
    }

    if (rmdir(folder_name) == -1) {
        perror("Failed to remove directory");
        exit(EXIT_FAILURE);
    }
}

int main() {
    generate_killer();
    while (1) {
        //create_folder();
        //download_IMAGES();
        //sleep(30);
        // Zip the folder and delete it
        char folder_name[100];
        time_t current_time = time(NULL);
        struct tm* time_info = localtime(&current_time);
        strftime(folder_name, sizeof(folder_name), FOLDER_NAME_FORMAT, time_info);
        create_folder(folder_name);
        download_IMAGES();
	create_zip(folder_name);
        remove_folder(folder_name);
	sleep(30);
    }
    return 0;
}

