#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void
clipboard_daemon()
{
    setsid();
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_BLOCK, &set, NULL);

    char *copy_cmd[] = {"/bin/sh", "-c", "xclip -selection primary -out | xclip -selection clipboard -in", NULL};
    char *clip_out = "xclip -selection clipboard -out";
    char *paste_cmd[] = {"/usr/bin/xdotool", "type", "--clearmodifiers", "--delay=0", NULL, NULL};
    int i = 4; // "something to type" index (see man xdotool)

    int sig, status;
    while (1) {
        sigwait(&set, &sig);
        switch(sig) {
            case SIGUSR1: // COPY
                if (fork() == 0) {
                    execv(copy_cmd[0], copy_cmd);
                    exit(EXIT_FAILURE);
                }
                waitpid(-1, &status, 0);
                break;
            case SIGUSR2: // PASTE
                FILE* clip = popen(clip_out, "r");
                if (clip == NULL) break;

                int n = 256;
                int bytes_read;
                int total_bytes_read = 0;
                int bytes_to_read = n - total_bytes_read - 1;
                paste_cmd[i] = malloc(n * sizeof(char));
                while ( (bytes_read = fread(paste_cmd[i] + total_bytes_read, 1, bytes_to_read, clip)) == bytes_to_read ) {
                    total_bytes_read += bytes_read;
                    n += n;
                    bytes_to_read = n - total_bytes_read - 1;
                    char* temp = malloc(n * sizeof(char));
                    strcpy(temp, paste_cmd[i]);
                    free(paste_cmd[i]);
                    paste_cmd[i] = temp;
                }
                total_bytes_read += bytes_read;
                paste_cmd[i][total_bytes_read] = '\0';
                pclose(clip);

                if (fork() == 0) {
                    execv(paste_cmd[0], paste_cmd);
                    exit(EXIT_FAILURE);
                }
                waitpid(-1, &status, 0);
                free(paste_cmd[i]);
                paste_cmd[i] = NULL;
                break;
        }
    }
}

