#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <math.h>

int random_in_range(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void onInterrupt(int sig) {
    printf("\033[?25h"); // Show Cursor
    exit(0);
}

void help();
void sideScrollerAnim();
void mathVis(int argc, char *argv[]);
double mathGetFuncValue(double x, char func[]);

int main(int argc, char *argv[]) {

    signal(SIGINT, onInterrupt);
    srand(time(NULL));

    if (argc < 2)   help();
    else {
        if (strcmp(argv[1], "sidescroller") == 0) {
            sideScrollerAnim();
        }
        else if (strcmp(argv[1], "math") == 0) {
            mathVis(argc, argv);
        }
        else {  // Default Case
            help();
        }
    }

    printf("\033[?25h"); // Show Cursor

    return 0;
}

void help() {
    printf("\tList of commands available:\n");
    printf("\t-------------------------------------------\n\n");
    printf("\tsidescroller\t:\ta simple side scrolling animation\n");
    printf("\tmath\t:\tmath function visualizer (type math -l for list of math functions available)\n");
}

void mathHelp() {
    printf("\tList of available math functions:\n");
    printf("\t------------------------------------------\n\n");
    printf("\tlinear\t\texponential\n");
    printf("\tsine\t\tcos\n");
    printf("\tcosec\t\tsec\n");
    printf("\ttan\t\tcot\n");
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

void mathVis(int argc, char *argv[]) {
    if (argc < 3 || strcmp(argv[2], "-l") == 0) {
        mathHelp();
        return;
    }

    struct winsize w;

    printf("\033[?25l");
    printf("\033[2J\033[3J\033[H");

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        for (int i = 1; i <= (w.ws_col); i++) {
            double x = (((double) (i - 1) / w.ws_col) * 20.0) - 10.0;
            int row = (int) (mathGetFuncValue(x, argv[2]) * (w.ws_row / 6));
            row = row + (w.ws_row / 2);
            row = w.ws_row - row;
            // printf("%d\t", row);
            // printf("(%d, %d)\n", row, i);
            if (row > 0 && row < w.ws_row) {
                printf("\033[%d;%dH", row, i);
                printf("█");
            }
        }

        printf("\033[%d;%dH", w.ws_row - 1, 1);
    }
    else {
        perror("ioctl");
    }


    return;
}

double mathGetFuncValue(double x, char func[]) {

    if (strcmp(func, "exponential") == 0) {
        return pow(x, 2);
    }
    else if (strcmp(func, "linear") == 0) {
        return x;
    }
    else if (strcmp(func, "sine") == 0) {
        return sin(x);
    }
    else if (strcmp(func, "cos") == 0) {
        return cos(x);
    }
    else if (strcmp(func, "tan") == 0) {
        return tan(x);
    }
    else if (strcmp(func, "cosec") == 0) {
        return 1.0/mathGetFuncValue(x, "sine");
    }
    else if (strcmp(func, "sec") == 0) {
        return 1.0/mathGetFuncValue(x, "cos");
    }
    else if (strcmp(func, "cot") == 0) {
        return 1.0/mathGetFuncValue(x, "tan");
    }
    else {
        return x;
    }
}