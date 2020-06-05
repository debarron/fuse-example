LDFLAGS=`pkg-config fuse3 --cflags --libs`

fuse_example_fs.o: 
	gcc -Wall -lstdc++ -std=c++11 src/fuse_example_fs.cpp -Isrc/ -g -c -o lib/fuse_example_fs.o

fuse_example_main.o:
	gcc -Wall src/fuse_example_main.c $(LDFLAGS) -Isrc/ -g -c -o lib/fuse_example_main.o

fuse-example: fuse_example_fs.o fuse_example_main.o
	gcc -Wall lib/fuse_example_fs.o lib/fuse_example_main.o $(LDFLAGS) -Isrc/ -o bin/fuse-example

