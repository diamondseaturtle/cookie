# lol makefiles

gcc storage.c -o storage
./storage
valgrind --leak-check=full ./storage