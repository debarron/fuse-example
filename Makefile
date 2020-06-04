LDFLAGS = `pkg-config fuse3 --cflags --libs`

fuse_example: 
	gcc -Wall src/fuse_example_main.c $(LDFLAGS) -o bin/fuse_example
