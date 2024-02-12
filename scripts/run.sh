gcc -c main.c -o main_exec.o
gcc -c twin_buffer.c -o twin_buffer_exec.o
gcc main_exec.o twin_buffer_exec.o -o driver_exec
./driver_exec

# Remove object files
rm -f *.o
#removes executable files
find . -perm +100 -type f -delete