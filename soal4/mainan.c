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

void handle_signal(int signal) {
  printf("Terminating the program...\n");
  exit(0);
}

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

// running bash script
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

int main(int argc, char *argv[]) {
  pid_t pid, sid;
  int hour, minute, second;
  char script_path[256];
  time_t now;
  struct tm *tm_info;
  char buffer[80];

  // Parse arguments
  parse_arguments(argc, argv, &hour, &minute, &second, script_path);

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
}