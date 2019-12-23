
fuse: 
	gcc -Wall fuse_example_main.c `pkg-config fuse3 --cflags --libs` -o fuse


