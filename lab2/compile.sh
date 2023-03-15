gcc -Wall -c functions.c -o functions.o
gcc -Wall -c main.c -o main.o
gcc main.o functions.o -o program
chmod +x program
./program
