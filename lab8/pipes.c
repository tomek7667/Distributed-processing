int main(int argc, char* argv[]) {
    int fd[2];
    // fd[0] is for reading
    // fd[1] is for writing
    if (pipe(fd) == -1) {
        printf("Pipe failed\n");
        return 1;
    }
    int id = fork();
    if (id == 0) {
        close(fd[0]);
        int x;
        printf("Enter a number: ");
        scanf("%d", &x);
        write(fd[1], &x, sizeof(int));
        close(fd[1]);
    } else {
        close(fd[1]);
        int y;
        read(fd[0], &y, sizeof(int));
        close(fd[0]);
        printf("Got from child process %d\n", y);
    }
    return 0;
}
