gcc -Wall -fPIC -c functions.c
gcc functions.o -shared -o lib_functions.so
gcc main.o -L. lib_functions.so -o program_dynamic
ldd ./program_dynamic
chmod +x ./program_dynamic
export LD_LIBRARY_PATH=.:"$LD_LIBRARY_PATH"
./program_dynamic
