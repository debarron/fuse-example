LDFLAGS=`pkg-config fuse3 --cflags --libs`

C_STR_FUNCTIONS_HOME = modules/c-str-functions
C_DATA_STRUCUTURES_HOME = modules/c-data-structures

INCLUDE_SUBMODULES_LIB = -I$(C_STR_FUNCTIONS_HOME)/lib -I$(C_DATA_STRUCUTURES_HOME)/lib
INCLUDE_SUBMODULES_SRC = -I$(C_STR_FUNCTIONS_HOME)/src -I$(C_DATA_STRUCUTURES_HOME)/src

str_functions.o:
	cd $(C_STR_FUNCTIONS_HOME) && make str_functions.o -f ./Makefile

tree.o:
	cd $(C_DATA_STRUCUTURES_HOME) && make tree.o -f ./Makefile

fuse_example_fs.o: 
	gcc -Wall -lstdc++ -std=c++11 src/fuse_example_fs.cpp -Isrc/ -g -c -o lib/fuse_example_fs.o

cpp_example.o: 
	gcc -Wall -lstdc++ -std=c++11 src/cpp_example.cpp -Isrc/ -g -c -o lib/cpp_example.o

fuse_example_main.o:
	gcc -Wall src/fuse_example_main.c $(LDFLAGS) -Isrc/ -g -c -o lib/fuse_example_main.o

fuse-example: fuse_example_fs.o fuse_example_main.o cpp_example.o
	g++ -Wall lib/fuse_example_fs.o lib/cpp_example.o lib/fuse_example_main.o $(LDFLAGS) -Isrc/ -o bin/fuse-example

