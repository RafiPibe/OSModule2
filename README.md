 # <b><i>sisop-praktikum-modul-02-2023-IF184402-UO4</i></b>

## Group UO4

## Revision link : [docs](https://docs.google.com/document/d/1sRkKg1hEMAZ4cpQFHhnYShozLv2VY8anvb-2mTbhYBs/edit?usp=sharing)
This is our attempt on the 2nd Practicum of Operation System 
consisting the members as following:

| Name                        | NRP        |
|-----------------------------|------------|
|Hanifi Abrar Setiawan        | 5025211066 |
|Faraihan Rafi Adityawarman   | 5025211074 |
|Vija Wildan Gita Prabawa     | 5025211261 |

## <b>1st Problem</b>

Grape-kun adalah seorang penjaga hewan di kebun binatang, dia mendapatkan tugas dari atasannya untuk melakukan penjagaan pada beberapa hewan-hewan yang ada di kebun binatang sebelum melakukan penjagaan Grape-kun harus mengetahui terlebih dahulu hewan apa aja yang harus dijaga dalam drive kebun binatang tersebut terdapat folder gambar dari hewan apa saja yang harus dijaga oleh Grape-kun.

#### Starter

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
```

Adding the necessary library for the program.

#### A

Grape-kun harus mendownload file tersebut untuk disimpan pada penyimpanan local komputernya. Dan untuk melakukan melihat file gambar pada folder yang telah didownload Grape-kun harus melakukan unzip pada folder tersebut.

#### Answer

```c
void downloadAndUnzip() {
...
}
```

We add the function downloadAndUnzip().

```c
pid_t pidDownload, pidExtract;
int status;
char *downloadArgs[] = {"wget", "-O", "binatang.zip", "https://drive.google.com/uc?export=download&id=1oDgj5kSiDO0tlyS7-20uz7t20X3atwrq", NULL};
char *unzipArgs[] = {"unzip", "binatang.zip", NULL};
```

Then we add the variables with pointer so that it'll accessable in the next lines.

```c
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
```

- `fork()`: Used to create a child process to run the download command using the downloadArgs variable.
- `execvp()`: called by the child process to run the "download" command.

```c
// Wait for the download process to finish
    waitpid(pidDownload, &status, 0);

// Wait for the extract process to finish
    waitpid(pidExtract, &status, 0);
```

This two function is used to wait until the process is finished.

```c
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
```

- `fork()`: Used to create a child process to run the extract command using the unzipArgs variable.
- `execvp()`: called by the child process to run the "unzip" command.

```c
// Remove the zip file
    if (remove("binatang.zip") != 0) {
        perror("remove error");
    }
```

because we already have the zip files and extracted the zip, then we wanted to remove the downloaded zip using the remove function.

#### B

Setelah berhasil melakukan unzip Grape-kun melakukan pemilihan secara acak pada file gambar tersebut untuk melakukan shift penjagaan pada hewan tersebut.

#### Answer

```c
void randomPicture() {
...
}
```

We add the function randomPicture.

```c
    DIR *dir;
    struct dirent *ent;
    int pictureNum = 0;
    char **picturePath;
    char *ext;
    int i, index;
    time_t t;
```

Then we add the variables with pointer so that it'll accessable in the next lines.

```c
// randomize using time
    srand((unsigned) time(&t));
```

we will use time using `time` to randomize the picking using `srand`

```c
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
```

Open and read the directory listed with what is inside that contains .jpg (the animals) to allocate memory for the array.
Then if there's no file, it will return "No pictures found"

```c
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
```

`rewinddir`: resets the directory stream dir to the beginning of the directory, so that the next call to readdir() will start from the first entry.
then it will read again because we already allocated the memory needed, and now we store the file names in the array.
after that we randomize the list, and finally print out one of the name of the picture.

```c
// free the memory
    closedir(dir);
    free(picturePath);
```

after that we free the memory so that if we want to do it again it doesn't have the previous allocated memory. 

#### C

Karena Grape-kun adalah orang yang perfeksionis Grape-kun ingin membuat direktori untuk memilah file gambar tersebut. Direktori tersebut dengan nama HewanDarat, HewanAmphibi, dan HewanAir. Setelah membuat direktori tersebut Grape-kun harus melakukan filter atau pemindahan file gambar hewan sesuai dengan tempat tinggal nya.

#### Answer

```c
void filterFile() {
    ...
}
```

We add the function randomPicture.

```c
// Open the directory containing the unzipped pictures
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir error");
        exit(EXIT_FAILURE);
    }
```

we open the directory that have the .jpg file inside.

```c
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
```

we then make a new directories/folder for each animal types if there's none yet.
`F_OK` is a flag that can be passed to the `access()` function systems to check if a file or directory exists. it is used to check if the directories of each animal types exist in the current directory. If the directory does not exist, `access()` returns -1, and the program creates the directory using mkdir(). If the directory already exists, `access()` returns 0, and the program does not create a new directory.

```c
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
            }
        }
    }
    // Close the directory
    closedir(dir);
```

This code iterates through files in a directory and moves files containing "darat", it then used in the other animal types (air and amphibi) in their names to corresponding directories using the mv command. It uses `entry->d_type` to check if the file is a regular file, and strstr() to check the file name for the required strings. `execvp()` is used to execute the mv command in a child process, while `fork()` creates the child process. Then we close the directory.

#### D

Setelah mengetahui hewan apa saja yang harus dijaga Grape-kun melakukan zip kepada direktori yang dia buat sebelumnya agar menghemat penyimpanan.

#### Answer

```c
void zipDirectory(char *directory) {
    ...
}
```

We add the function randomPicture.

```c
    pid_t pid;
    int status;

    // Construct the command to zip the directory
    char *zipArgs[] = {"zip", "-r", "-q", "-9", directory, directory, NULL};
```

Then we add the variables with pointer so that it'll accessable in the next lines.

```c
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
```

- `fork()`: Used to create a child process to run the zip command using the zipArgs variable.
- `execvp()`: called by the child process to run the "zip" command.

```c
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

```

This code removes a directory using the `rm` command because we already have the files zipped in 3 different animal types. It first creates an array of arguments for the rm command that includes the directory name and the `-rf` flag to recursively remove the directory and its contents. Then it forks a child process and uses `execvp()` to replace the child process with the rm command, passing in the rmArgs array as the command and arguments. If `execvp()` fails, it prints an error message and exits with a failure status. If `fork()` fails, it also prints an error message and exits with a failure status.

```c
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
```

calls the all the functions in the main function.

## <b>2nd Problem</b>

Sucipto adalah seorang seniman terkenal yang berasal dari Indonesia. Karya nya sudah terkenal di seluruh dunia, dan lukisannya sudah dipajang di berbagai museum mancanegara. Tetapi, akhir-akhir ini sucipto sedang terkendala mengenai ide lukisan ia selanjutnya. Sebagai teman yang jago sisop, bantu sucipto untuk melukis dengan mencarikannya gambar-gambar di internet sebagai referensi !

#### A

Buatlah sebuah folder khusus, yang dalamnya terdapat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-MM-dd_HH:mm:ss].

#### Answer

```
void create_folder() {
    time_t current_time = time(NULL);
    struct tm* time_info = localtime(&current_time);
    char folder_name[100];
    strftime(folder_name, sizeof(folder_name), FOLDER_NAME_FORMAT, time_info);
    mkdir(folder_name, 0755);
    chdir(folder_name);
}
```

#### Explanation

- void create_folder() {: This line defines the function named create_folder which takes no arguments and returns nothing (void).

- time_t current_time = time(NULL);: This line creates a time_t variable named current_time and assigns it the current time using the time function. The NULL argument tells time to use the system's current time.

- struct tm* time_info = localtime(&current_time);: This line creates a pointer to a struct tm named time_info and assigns it the local time corresponding to the current_time variable using the localtime function.

- char folder_name[100];: This line creates an array of characters named folder_name with a length of 100. This array will be used to store the name of the folder we want to create.

- strftime(folder_name, sizeof(folder_name), FOLDER_NAME_FORMAT, time_info);: This line formats the time stored in time_info into a string using the strftime function. The resulting string is stored in folder_name. FOLDER_NAME_FORMAT is a macro that specifies the format for the folder name string.

- mkdir(folder_name, 0755);: This line creates a new directory with the name stored in folder_name using the mkdir function. The 0755 argument specifies the permissions for the new directory.

- chdir(folder_name);: This line changes the current working directory to the new directory we just created using the chdir function.


#### B

Tiap-tiap folder lalu diisi dengan 15 gambar yang di download dari https://picsum.photos/ , dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+50 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:mm:ss].

#### Answer

```
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
```

#### Explanation

- The function download_IMAGES() is defined with no input arguments and no return value.
- A for loop is used to iterate over a sequence of 15 integers, stored in the variable image_count, ranging from 0 to 14 inclusive.
- The current time is obtained using time(&current_time).
- The struct tm pointer time_info is created and assigned the value of the local time using localtime(&current_time).
- An array of characters, file_name, is created with a length of 80.
- Another array of characters, url, is created with a length of 50.
- The strftime() function is used to format the current time as a string and store it in file_name.
- The sprintf() function is used to format the URL for the image and store it in url.
- A file with the name stored in file_name is created using fopen(), with the mode "wb" (write binary).
- If the file pointer returned by fopen() is NULL, an error message is printed using perror() and the program exits with an error   code using exit().
- A message is printed to the console indicating that the image is being downloaded, using printf().
- The standard output is flushed using fflush().
- A child process is created using fork(), and the process ID (PID) of the child is stored in pid.
- If pid is less than zero, an error message is printed and the program exits with an error code.
- If pid is zero, then this is the child process. The child process first formats a string containing the dimensions of the image to be downloaded, using sprintf().
- The execl() function is then used to execute the command-line program /usr/bin/wget, passing it the arguments -q, -O, file_name, and url. This downloads the image to the file specified by file_name.
- Another execl() function call is made, this time to the program /usr/bin/convert, passing it the arguments -resize, size_option, file_name, and file_name. This resizes the image to the dimensions specified by size_option and overwrites the original file with the resized image.
- If pid is not zero, then this is the parent process. The parent process closes the file using fclose() and then waits for 5 seconds using sleep().
- After the for loop completes, a while loop is used to wait for all child processes to complete, using wait().

#### C

Agar rapi, setelah sebuah folder telah terisi oleh 15 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).

#### Answer

```
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
    
```

#### Explanation
Function create_zip

- Takes a string folder_name as input which specifies the name of the folder that needs to be compressed.
- Creates a string variable zip_name by appending ".zip" to the folder_name.
- Creates a string variable zip_command that stores the command to compress the folder using the zip command-line utility.
- Executes the zip command using the system function and passes zip_command as argument to it.
- If the system function fails to execute the zip command, it prints an error message and terminates the program.

Function remove_folder

- Takes a string folder_name as input which specifies the name of the folder that needs to be removed.
- Opens the directory using the opendir function and returns a pointer to the directory stream.
- Reads the directory entries one by one using the readdir function and stores the entry information in a dirent structure.
- For each directory entry, it creates a string variable path that specifies the full path of the file/directory.
- Determines the file type of the entry (directory or file) using the stat function.
- If the entry is a directory, it recursively calls the remove_folder function to delete the directory and its contents.
- If the entry is a file, it deletes the file using the unlink function.
- Closes the directory using the closedir function.
- Deletes the folder using the rmdir function.
- If any of the above steps fail, it prints an error message and terminates the program.


#### D

Karena takut program tersebut lepas kendali, Sucipto ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.

#### Answer

```
void generate_killer() {
    char kill_prog[] = "(compiled-filed-name)";
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
```

#### Explanation

- This function generates a shell script named "killer.sh" by opening a file in write mode using fopen function.

- The first line of the shell script is "#!/bin/bash", which specifies the shell that should be used to run the script.

- The next line of the shell script kills the process with the specified PID using the kill command.

- If the mode parameter is set to 'A', the shell script deletes the executable file specified in program_name using the rm command.

- If the mode parameter is set to 'B', the shell script deletes the entire directory specified in program_name and its contents using the rm command with the "-rf" option.

- The last line of the shell script deletes itself using the rm command.

- The fclose function is called to close the file.

- The function creates a child process using fork function.

- If the child process is created successfully (i.e., if the return value of fork is 0), the child process changes the file mode of "killer.sh" to be executable using the chmod command-line utility by calling the execlp function.

- If the child process is not created successfully (i.e., if the return value of fork is not 0), the parent process waits for the child process to finish using the wait function before continuing.

- Once the shell script is generated and made executable (if necessary), the function prints a message to the console indicating that the killer file has been generated and provides instructions for running the script to terminate the program.

#### E

Buatlah program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).

#### Answer

```

```

#### Explanation


## <b>3rd Problem</b>

Ten Hag adalah seorang pelatih Ajax di Liga Belanda. Suatu hari, Ten Hag mendapatkan tawaran untuk menjadi manajer Manchester United. Karena Ten Hag masih mempertimbangkan tawaran tersebut, ia ingin mengenal para pemain yang akan dilatih kedepannya. Dikarenakan Ten Hag hanya mendapatkan url atau link database mentah para pemain bola, maka ia perlu melakukan klasifikasi pemain Manchester United. Bantulah Ten Hag untuk mengenal para pemain Manchester United tersebut hanya dengan 1 Program C bernama “filter.c”

#### Starter

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>


#### Explanation

This code is for importing the library so the function can run

#### A

Pertama-tama, Program filter.c akan mengunduh file yang berisikan database para pemain bola. Kemudian dalam program yang sama diminta dapat melakukan extract “players.zip”. Lalu hapus file zip tersebut agar tidak memenuhi komputer Ten Hag.

#### Answer

``` C
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


```

#### Explanation

- Declare ```pid_t``` variables ```pidDownload``` and ```pidExtract``` to store the process IDs of the child processes.
- Declare an int variable status to store the exit status of the child processes.
- Declare two char arrays ```downloadArgs``` and ```unzipArgs``` to store the command line arguments for the ```wget``` and unzip commands respectively.
- Fork a child process to run the ```wget``` command using the ```downloadArgs``` array.
- Check if the fork was successful, and if not, print an error message and exit with a failure code.
- In the child process, execute the ```wget``` command using the ```execvp()``` function.
- If the``` execvp()``` function returns, print an error message and exit with a failure code.
- In the parent process, wait for the ```wget``` child process to finish using the ```waitpid()``` function, and store the exit status in status.
- Fork another child process to run the unzip command using the ```unzipArgs``` array.
- Check if the fork was successful, and if not, print an error message and exit with a failure code.
- In the child process, execute the unzip command using the ```execvp()``` function.
- If the ```execvp()``` function returns, print an error message and exit with a failure code.
- In the parent process, wait for the unzip child process to finish using the ```waitpid()``` function, and store the exit status in status.
- Remove the database.zip file using the ```remove()``` function.
- If the ```remove()``` function returns a non-zero value, print an error message.

#### B

Dikarenakan database yang diunduh masih data mentah. Maka bantulah Ten Hag untuk menghapus semua pemain yang bukan dari Manchester United yang ada di directory.  

#### Answer

```C
void deleteNonManUtdFiles() {
    DIR *dir;
    struct dirent *entry;
    char *match = "ManUtd";
    char *filename;
    char filepath[256];

    // Open the directory path
    dir = opendir("/home/eezr/ngoding/rawr/players/");
    if (dir == NULL) {
        perror("opendir error");
        exit(EXIT_FAILURE);
    }

    // To delete files that doesn't contain 'ManUtd' in the name
    while ((entry = readdir(dir)) != NULL) {
        filename = entry->d_name;
        if (strstr(filename, match) == NULL) {
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

```

#### Explanation

- The ```deleteNonManUtdFiles()``` function deletes files in a directory that do not contain the string "ManUtd" in their filename.
- It opens the directory ```/home/eezr/ngoding/rawr/players/``` using the ```opendir()``` function.
- It checks each file in the directory using ```readdir()``` function.
- If the file name does not contain the string "ManUtd" using ```strstr()``` function, the function constructs the full file path using ```snprintf()``` function and attempts to delete the file using the ```remove()``` function.
- If the deletion is successful, it prints a message to the console with the name of the deleted file.
- If the deletion fails, it prints an error message using ```perror()``` function.
- Once all files have been processed, it closes the directory using ```closedir()``` function.


#### C

Setelah mengetahui nama-nama pemain Manchester United, Ten Hag perlu untuk mengkategorikan pemain tersebut sesuai dengan posisi mereka dalam waktu bersamaan dengan 4 proses yang berbeda. Untuk kategori folder akan menjadi 4 yaitu Kiper, Bek, Gelandang, dan Penyerang.

#### Answer

```C
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

```

#### Explanation
- The ```categorizePlayers()``` function opens a directory ```/home/eezr/ngoding/rawr/players/``` containing files related to ManUtd players.
- It iterates over each file in the directory using ```readdir()``` function.
- For each file, it determines the position of the player based on their name by checking for keywords like Kiper, Bek, Gelandang, and Penyerang in the filename using ```strstr()``` function.
- If the file is not related to a "ManUtd" player, it skips to the next file using the continue statement.
- It creates a new directory for the player's position if it doesn't exist using ```fork()``` and ```execvp()``` functions to run the mkdir command.
- It moves the player's file to the appropriate position folder using ```fork()``` and ```execvp()``` functions to run the mv command.
- It waits for the child process to complete using the ```waitpid()``` function.
- Once all files have been processed, it closes the directory using ```closedir()``` function.


## <b>4th Problem</b>

Banabil adalah seorang mahasiswa yang rajin kuliah dan suka belajar. Namun naasnya Banabil salah mencari teman, dia diajak ke toko mainan oleh teman-temannya dan teracuni untuk membeli banyak sekali mainan dan kebingungan memilih mainan mana yang harus dibeli. Hal tersebut menyebabkan Banabil kehilangan fokus dalam pengerjaan tugas-tugas yang diberikan oleh dosen nya. Untuk mengembalikan fokusnya, Banabil harus melatih diri sendiri dalam membuat program untuk menjalankan script bash yang menyerupai crontab dan menggunakan bahasa C karena baru dipelajari olehnya.

#### Starter

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
```

#### Explanation

Adding the necessary library for the program.

```c
void handle_signal(int signal) {
  printf("Terminating the program...\n");
  exit(0);
}
```

#### Explanation

This function handles the SIGTERM signal and terminates the program by printing a message and exiting.

```c
void parse_arguments(int argc, char *argv[], int *hour, int *minute, int *second, char *script_path) {
  if (argc != 5) {
    printf("Usage: %s <hour> <minute> <second> <script_path>\n", argv[0]);
    exit(1);
  }

  *hour = atoi(argv[1]);
  *minute = atoi(argv[2]);
  *second = atoi(argv[3]);
  strcpy(script_path, argv[4]);

  if (*hour < 0 || *hour > 23) {
    printf("Error: hour must be between 0 and 23\n");
    exit(1);
  }

  if (*minute < 0 || *minute > 59) {
    printf("Error: minute must be between 0 and 59\n");
    exit(1);
  }

  if (*second < 0 || *second > 59) {
    printf("Error: second must be between 0 and 59\n");
    exit(1);
  }
}

```

#### Explanation

parse_arguments is used to parse the command-line arguments, validates them, and assigns their values to the corresponding variables.

it first checks if the number of arguments is equal to 5. If not, it prints the correct usage of the program and exits with a failure status.
If the number of arguments is correct, the function extracts the values for hour, minute, and second from the argument vector using the atoi function to convert the string to integers. It also copies the script path argument to the script_path variable using the strcpy function.
The function then checks if the values for hour, minute, and second are within the correct range (hour between 0 and 23, minute and second between 0 and 59). If any of these values are not within the correct range, the function prints an error message and exits with a failure status.

```c
void run_script(const char *script_path) {
    int status;
    pid_t pid = fork();

    // child process
    if (pid == 0) {
        execl("/bin/sh", "sh", script_path, (char *)NULL);
        exit(EXIT_FAILURE);
    } 
    else if (pid < 0) {
        syslog(LOG_ERR, "Fork failed");
    } else {
        waitpid(pid, &status, 0);
    }
}
```

#### Explanation

The `run_script` function forks a child process and executes a bash script using the `execl` function. If the child process is successfully created, it replaces itself with a new program specified by the script path passed as an argument using `execl`. If `execl` fails, it exits with a failure status. If `fork` fails, it logs an error using `syslog`. The parent process waits for the child process to complete using `waitpid`.

```c
int main(int argc, char *argv[]) {
    pid_t pid, sid;
  int hour, minute, second;
  char script_path[256];
  time_t now;
  struct tm *tm_info;
  char buffer[80];

  // Parse arguments
  parse_arguments(argc, argv, &hour, &minute, &second, script_path);
  ...
}
```

#### Explanation

adding variables and call the arguments

```c
pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  } 
  else if (pid > 0) {
    exit(EXIT_FAILURE);
  }

  // Set new file mode
  umask(0);

  // New session
  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  // Change to root directory
  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }
```

#### Explanation

Fork a new process and terminate the parent process.
Set the new file mode using umask(0).
Create a new session using setsid() to detach the daemon from the terminal.
Change the current working directory to the root directory using chdir("/").

```c
// Infinite loop
  while (1) {
        // Taking the current time
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        // Check designated time
        if ((hour == -1 || tm.tm_hour == hour) && tm.tm_min == minute && tm.tm_sec == second) {
            run_script(script_path);
        }
        sleep(1);
    }

  return EXIT_SUCCESS;
```

#### Explanation

Enter an infinite loop that continuously checks the current time and runs the script if the designated time matches the current time, using the run_script function then sleep for 1 second to reduce CPU usage.

