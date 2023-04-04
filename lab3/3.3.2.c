#include<unistd.h>
#include<stdio.h>

int main() {
    int pid = fork();
    if (pid == 0) {
        printf("proces potomny pid: %d ppid: %d\n", getpid(), getppid());
        execlp("ps", "ps", "-u", "codespace", "-l", "--forest", NULL);
        sleep(5);
        return 0;
    } else {
        // It waits for the child process to finish.
        waitpid(pid, NULL, 0);
        printf("proces macierzysty pid: %d ppid: %d\n", getpid(), getppid());
        return 0;
    }
}
