// Hunter Johnson
// COP 3502, Fall 2018
// NID | hu066164

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Glowworm.h"

double difficultyRating(void) {
    return 4.0;
}

double hoursSpent(void) {
    return 22.0;
}
/* -Prints the glowworm based on values of conditonals
 * received from the logic within main function.
 * -Output is formatted to print the behavior followed
 * by newline then glowworm followed by newline then
 * ledge the glowworm sits on followed by 2 newlines
 * tail: position of tail character '~'
 * head: position of head character 'G'
 * maxLength: maximum length of glowworm and ledge
 * dead: boolean used to track if glowworm is alive
 */
void printGlowworm(int tail, int head, int maxLength, int dead) {
    int i;
    
    for (i = 0; i < maxLength; i++) {
        if (tail < head) { // normal mode, no wrap
            if (i == tail) {
                printf("~");
            } else if (i == head) {
                if (dead == 1) {
                    printf("x");
                } else {
                    printf("G");
                }
            } else if (i > tail && i < head && (head - 1) == i) {
                if (dead == 1) {
                    printf("X");
                } else {
                    printf("O");
                }
            } else if (i > tail && i < head) {
                printf("o");
            } else if ((i - tail) != 0 && i < head) {
                printf(" ");
            }
        } else { // magical mode, wrap
            if (i == tail) {
                printf("~");
            } else if (i == head) {
                if (dead == 1) {
                    printf("x");
                } else {
                    printf("G");
                }
            } else if (i == (head - 1) + maxLength) {
                if (dead == 1) {
                    printf("X");
                } else {
                    printf("O");
                }
            } else if (i < head && (head - 1) == i) {
                if (dead == 1) {
                    printf("X");
                } else {
                    printf("O");
                }
            } else if (i > tail && i < ((head - 1) + maxLength)) {
                printf("o");
            } else if (tail == (maxLength - 1) && i <= (head - 2)) {
                printf("o");
            } else if (i <= (head - 2)) {
                printf("o");
            } else if ((i - tail) != 0) {
                printf(" ");
            }
        }
    }
    printf("\n");
    for (i = 0; i < maxLength; i++) {
        printf("=");
    }
    printf("\n\n");
}

int main(int argc, char ** argv) {
    int maxLength;
    int i;
    int tail = 0;
    int head = 2;
    int dead = 0;
    
    maxLength = atoi(argv[1]); // maximum length of glowworm and ledge
    printf("%s\n\n", argv[2]);
    printf("Glowworm appears! Hooray!\n");
    printGlowworm(tail, head, maxLength, dead);
    
    /* Loops trhough string chars and processes
     * how glowworm should react. 5 main behaviors
     * Default: chillin', growing, shrinking, inching,
     * dying
     */
    for (i = 0; i < strlen(argv[2]); i++) {
        switch (argv[2][i]) {
            case 'o':
            case 'O':
            case '@':
                if (head % (maxLength - 1) == 0 && head == maxLength - 1 && tail != 0) {
                    head = head % (maxLength - 1);
                    printf("Glowworm grows:\n");
                    printGlowworm(tail, head, maxLength, dead);
                } else if (head != (maxLength - 1)) {
                    head++;
                    printf("Glowworm grows:\n");
                    printGlowworm(tail, head, maxLength, dead);
                } else {
                    printf("Glowworm chills:\n");
                    printGlowworm(tail, head, maxLength, dead);
                }
                break;
            case 's':
            case 'S':
                if ((head - tail - 2) != 0 && head != 0) {
                    head--;
                    printf("Glowworm shrinks:\n");
                    printGlowworm(tail, head, maxLength, dead);
                } else if (head == 0) {
                    head = (head - 1) + maxLength;
                    printf("Glowworm shrinks:\n");
                    printGlowworm(tail, head, maxLength, dead);
                } else {
                    printf("Glowworm chills:\n");
                    printGlowworm(tail, head, maxLength, dead);
                }
                break;
            case '-':
            case '=':
                if (head % (maxLength - 1) == 0 && head == maxLength - 1) {
                    tail++;
                    head = head % (maxLength - 1);
                    printf("Glowworm inches forward:\n");
                    printGlowworm(tail, head, maxLength, dead);
                } else if (tail % (maxLength - 1) == 0 && tail == maxLength - 1) {
                    tail = tail % (maxLength - 1);
                    head++;
                    printf("Glowworm inches forward:\n");
                    printGlowworm(tail, head, maxLength, dead);
                } else {
                    tail++;
                    head++;
                    printf("Glowworm inches forward:\n");
                    printGlowworm(tail, head, maxLength, dead);
                }
                break;
            case 'x':
            case 'X':
            case '%':
                dead++;
                printf("Glowworm meets its demise. SAD.\n");
                printGlowworm(tail, head, maxLength, dead);
                return 0;
                break;
            default:
                printf("Glowworm chills:\n");
                printGlowworm(tail, head, maxLength, dead);
        }
    }
    return 0;
}
