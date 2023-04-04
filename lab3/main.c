#include <stdio.h>
#include <stdlib.h>

void printPs(int n) {
    if (n == 0) {
        sleep(5);
        exit(0);
    }
    // printf("me=%d; my parent=%d\n", getpid(), getppid());
    int treeChildren = 2;
    for (int i = 0; i < treeChildren; i++) {
        int pid = fork();
        if (pid == 0) {
            printf("pid: %d | ppid: %d\n", getpid(), getppid());
            printPs(n - 1);
            sleep(5);
            return;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Please specify only one argument of type int.\n");
        return 1;
    }
    int userInput = atoi(argv[1]);
    printf("userInput = %d\n", userInput);
    if (userInput < 0) {
        printf("Please specify a positive integer.\n");
        return 1;
    }
    int pid = getpid();
    printPs(userInput);
    sleep(5);
    // waitpid(pid, NULL, 0);
    if (pid == getpid()) {
        printf("I am the parent process. My pid is %d\n", pid);
        execlp("ps", "-u codespace", "--forest", NULL);
    }
    return 0;
}
