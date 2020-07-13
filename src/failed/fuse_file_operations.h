/*
 *
 * */

#ifndef FUSE_FILE_OPERATIONS_H
#define FUSE_FILE_OPERATIONS_H

#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <libgen.h>
#include <errno.h>

#include "tree.h"
#include "fe_data.h"

#define FE_FILE_SIZE 1048576

extern tree_t *fs;

static void fe_set_file_stat(struct stat *v);
static void fe_set_dir_stat(struct stat *v);
static int fe_getattr(const char *path, struct stat *stbuf);
static int fe_open(const char *path, struct fuse_file_info *fi);
static int fe_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
static int fe_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);

#endif
