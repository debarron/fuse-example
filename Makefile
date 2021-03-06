
GIT_INIT_SUBMODULES = $(shell git submodule init && \
											git submodule update --init --recursive &&\
											git submodule update --remote)

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

git-submodules:
	echo $(GIT_INIT_SUBMODULES)

all: objects tests
.PHONY: $(dependencies) objects tests

objects: $(dependencies) fe_data.o fuse_example_main.o

tests: fe_data_test

$(dependencies): 
	$(MAKE) -C $@

fe_data.o: $(dependencies)
	gcc -o $(LOCAL_LIB)/fe_data.o \
		-c $(LOCAL_SRC)/fe_data.c \
		$(INCLUDE_ALL)

fe_data_test.o: fe_data.o
	gcc -o $(LOCAL_LIB)/fe_data_test.o \
		-c $(LOCAL_SRC)/fe_data_test.c \
		$(INCLUDE_ALL) 

fe_data_test: fe_data_test.o
	gcc -o $(LOCAL_BIN)/fe_data_test \
		$(LOCAL_LIB)/fe_data_test.o \
		$(LOCAL_LIB)/fe_data.o \
		$(INCLUDE_ALL) 


fuse_example_main.o: objects
	gcc -o $(LOCAL_LIB)/fuse_example_main.o -g \
		-c $(LOCAL_SRC)/fuse_example_main.c \
		$(LDFLAGS) $(INCLUDE_ALL)

fuse-example: fuse_example_main.o
	gcc -o $(LOCAL_BIN)/fuse-example \
		$(LOCAL_LIB)/fuse_example_main.o \
		./modules/c-data-structures/lib/tree.o \
		./lib/fe_data.o \
		-Wall $(LDFLAGS) \
		$(INCLUDE_ALL) 

