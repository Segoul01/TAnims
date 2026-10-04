#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <signal.h>
#include <string.h>

int random_in_range(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void onInterrupt(int sig) {
    printf("\033[?25h"); // Show Cursor
    exit(0);
}

void help();
void sideScrollerAnim();
void math(char func[]);

int main(int argc, char *argv[]) {

    signal(SIGINT, onInterrupt);
    srand(time(NULL));

    if (argc < 2)   help();
    else {
        if (strcmp(argv[1], "sidescroller") == 0) {
            sideScrollerAnim();
        }
        else {  // Default Case
            help();
        }
    }

    return 0;
}

void help() {
    printf("\tList of commands available:\n");
    printf("\t-------------------------------------------\n\n");
    printf("\tsidescroller\t:\ta simple side scrolling animation\n");
}

void sideScrollerAnim() {

    struct winsize w;

    printf("\033[?25l");

    int numOfGliders = 15;

    int gliders[numOfGliders];
    int gliderStartPoints[numOfGliders];
    int gliderStartRows[numOfGliders];
    
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        for (int i = 0; i < numOfGliders; i++) {
            gliderStartRows[i] = random_in_range(1, w.ws_row - 1);
            gliderStartPoints[i] = random_in_range(-300, 0);
            gliders[i] = gliderStartPoints[i];
        }
    }


    while (true) {

        printf("\033[2J\033[3J\033[H");

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {

            for (int i = 0; i < numOfGliders; i++) {
                gliders[i]++;

                if (gliders[i] >= w.ws_col - 1) {
                    gliders[i] = gliderStartPoints[i];
                }

                if (gliders[i] > 0) {
                    printf("\033[%d;%dH", gliderStartRows[i], gliders[i]);
                    printf("█\n");
                }

            }
        }
        else {
            perror("ioctl");
        }

        usleep(10000);
    }
}

void math(char func[]) {
    
}