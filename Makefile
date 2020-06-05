LDFLAGS=`pkg-config fuse3 --cflags --libs`


fuse_example_fs.o: 
	gcc -Wall -lstdc++ -std=c++11 src/fuse_example_fs.cpp -Isrc/ -g -c -o lib/fuse_example_fs.o

fuse-example: fuse_example_fs.o
	gcc -Wall lib/fuse_example_fs.o src/fuse_example_main.c $(LDFLAGS) -Isrc/ -o bin/fuse-example

