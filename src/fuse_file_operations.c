/*
 *
 * */

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

static void fe_set_file_stat(struct stat *v){
  v->st_mode = S_IFREG | 0444;
  v->st_nlink = 1;
  v->st_size = FE_FILE_SIZE;
}

static void fe_set_dir_stat(struct stat *v){
    v->st_mode = S_IFDIR | 0755;
		v->st_nlink = 2;
}

static int fe_getattr(
  const char *path, 
  struct stat *stbuf) 
{
  tree_t *entry;
  fe_data entry_info;

  entry = tree_find(fs, path);
  if(entry == NULL){
    errno = ENOENT;
    return -errno;
  }

  memset(stbuf, 0, sizeof(struct stat));
  entry_info = fe_data_from_void_ptr(entry->data);
  memcpy(sbuf, &entry_info.vstat, sizeof(struct stat));

  return 0;
}

static int fe_open(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> OPEN Opening a file, path=%s\n", path);

  // TODO This calls ramcloud read

  return status;
}


static int fe_write(
  const char *path, 
  const char *buf, 
  size_t size, 
  off_t offset, 
  struct fuse_file_info *fi
  ) 
{
  /*
  struct filehandle *fh = (struct filehandle *) fi->fh;

  // Check whether the file was opened for writing
  if(!O_WRITE(fh->o_flags)) {
    return -EACCES;
  }

  struct node *node = fh->node;

  // Calculate number of required blocks
  blkcnt_t req_blocks = (offset + size + BLOCKSIZE - 1) / BLOCKSIZE;

  if(node->vstat.st_blocks < req_blocks) {
    // Allocate more memory
    void *newdata = malloc(req_blocks * BLOCKSIZE);
    if(!newdata) {
      return -ENOMEM;
    }

    // Copy old contents
    if(node->data != NULL) {
      memcpy(newdata, node->data, node->vstat.st_size);
      free(node->data);
    }

    // Update allocation information
    node->data = newdata;
    node->vstat.st_blocks = req_blocks;
  }

  // Write to file buffer
  memcpy(((char *) node->data) + offset, buf, size);

  // Update file size if necessary
  off_t minsize = offset + size;
  if(minsize > node->vstat.st_size) {
    node->vstat.st_size = minsize;
  }

  update_times(node, U_CTIME | U_MTIME);

  */
  return size;
}


static int fe_read(
  const char *path,
  char *buf,
  size_t size,
  off_t offset,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, "NF >> READ path: %s\n", path);

  // TODO This calls ramcloud read

  return status;
}


