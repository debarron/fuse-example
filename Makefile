
dependencies = modules/c-str-functions modules/c-data-structures
dependencies_src = -Imodules/c-str-functions/src -Imodules/c-data-structures/src
dependencies_lib = -Imodules/c-str-functions/lib -Imodules/c-data-structures/lib
INCLUDE_SUBMODULES = $(dependencies_src) $(dependencies_lib)

LOCAL_LIB = ./lib
LOCAL_SRC = ./src
LOCAL_BIN = ./bin
INCLUDE_LOCAL = -I$(LOCAL_LIB) -I$(LOCAL_SRC)

INCLUDE_ALL = $(INCLUDE_LOCAL) $(INCLUDE_SUBMODULES)
LDFLAGS=`pkg-config fuse3 --cflags --libs`

GIT_INIT_SUBMODULES = $(shell git submodule init && \
											git submodule update --init --recursive &&\
											git submodule update --remote)

all: objects fuse_dependencies tests
.PHONY: $(dependencies) 

git-submodules:
	echo $(GIT_INIT_SUBMODULES)



objects: $(dependencies) fe_data.o

fuse_dependencies: fuse_file_operations.o fuse_example_main.o

tests: fe_data_test

$(dependencies): 
	$(MAKE) -C $@

fe_data.o: $(dependencies)
	gcc -o $(LOCAL_LIB)/fe_data.o -c $(LOCAL_SRC)/fe_data.c $(INCLUDE_ALL)

fe_data_test: fe_data.o
	gcc -o $(LOCAL_BIN)/fe_data_test $(LOCAL_SRC)/fe_data_test.c \
		$(INCLUDE_ALL) 

fuse_file_operations.o: objects
	gcc -o $(LOCAL_LIB)/fuse_file_operations.o -c $(LOCAL_SRC)/fuse_file_operations.c \
		$(INCLUDE_ALL)

fuse_example_main.o: fuse_file_operations.o
	gcc -o $(LOCAL_LIB)/fuse_example_main.o -g -c $(LOCAL_SRC)/fuse_example_main.c \
		$(LDFLAGS) $(INCLUDE_ALL)

fuse-example: fuse_dependencies
	g++ -o $(LOCAL_BIN)/fuse-example -Wall $(LOCAL_LIB)/fuse_example_main.o \
		$(LDFLAGS) $(INCLUDE_ALL)
	

#
#fuse_example_fs.o: 
#	gcc -Wall -lstdc++ -std=c++11 src/fuse_example_fs.cpp -Isrc/ -g -c -o lib/fuse_example_fs.o
#
#cpp_example.o: 
#	gcc -Wall -lstdc++ -std=c++11 src/cpp_example.cpp -Isrc/ -g -c -o lib/cpp_example.o
#
#fuse_example_main.o:
#	gcc -Wall src/fuse_example_main.c $(LDFLAGS) -Isrc/ -g -c -o lib/fuse_example_main.o
#
#fuse-example: fuse_example_fs.o fuse_example_main.o cpp_example.o
#	g++ -Wall lib/fuse_example_fs.o lib/cpp_example.o lib/fuse_example_main.o $(LDFLAGS) -Isrc/ -o bin/fuse-example
