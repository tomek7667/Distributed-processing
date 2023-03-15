ar r lib_functions.a functions.o
ar tv lib_functions.a
gcc main.o lib_functions.a -o program_static
chmod +x ./program_static
./program_static