/*
 * fuse_example_main.c
 * 12/17/2019
 * @debarron
 * A basic example on how to implement 
 * user-level file systems
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
#define THE_ROOT "/"

struct filesystem {
  tree_t *root;
};

struct filehandle {
  tree_t *node;
  int o_flags;
};

struct filesystem the_fs;

void fe_init(
  struct fuse_conn_info *conn,
  struct fuse_config *config)
{
  int status = 0;
  fprintf(stdout, ">> INIT Filesystem initiated/n");
  return NULL;
}

static int initstat(struct stat *stbuf, mode_t mode) {
  memset(stbuf, 0, sizeof(struct stat));
  stbuf->st_mode  = mode;
  stbuf->st_nlink = 0;
  return 1;
}

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

  fprintf(stdout, ">> FUNCTION: fe_getattr path='%s'\n", path);

  entry = tree_find(the_fs.root, path);
  if(entry == NULL){
    errno = ENOENT;
    return -errno;
  }

  memset(stbuf, 0, sizeof(struct stat));
  entry_info = fe_data_from_void_ptr(entry->data);
  memcpy(stbuf, &entry_info.vstat, sizeof(struct stat));

  return 0;
}

static int fe_open(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> FUNCTION: fe_open, path=%s\n", path);
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
  fprintf(stdout, ">> FUNCTION: fe_write path='%s'\n", path);
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
  return status;
}

static int fe_mknod(
    const char *path, 
    mode_t mode, 
    dev_t rdev) 
{
  fe_data new_entry;

  fprintf(stdout, ">> FUNCTION fe_mknod path='%s'\n", path);

  initstat(&new_entry.vstat, mode);
  tree_add(the_fs.root, path, fe_data_to_void_ptr(new_entry));
  
  return 0;
}


static int fe_readdir(
  const char *path,
  void *buffer,
  fuse_fill_dir_t filler,
  off_t offset,
  struct fuse_file_info *fi,
  enum fuse_readdir_flags flags)
{
  tree_t *entry, *parent, *child;
  fe_data entry_data, parent_data, child_data;
  
  entry = tree_find(the_fs.root, path);
  entry_data = fe_data_from_void_ptr(entry->data);
  if((entry != NULL) || (S_ISREG(entry_data.vstat.st_mode))){
    errno = ENOENT;
    return -errno;
  } 

  filler(buffer, ".",  &entry_data.vstat, 0, 0);
  if(strcmp(path, THE_ROOT) == 0) filler(buffer, "..", NULL, 0, 0);
  else{
    parent = tree_find_parent(the_fs.root, path);
    parent_data = fe_data_from_void_ptr(parent->data);
    filler(buffer, "..", &parent_data.vstat, 0, 0);
  }
    
  // fill_up_children
  child = entry->children;
  while(child != NULL){
    child_data = fe_data_from_void_ptr(child->data);
    if(filler(buffer, child->path, &child_data.vstat, 0, 0))
      break;

    child = child->next;
  }

  return 0;
}


static struct fuse_operations ramcloud_fuse_oper = {
  .getattr = fe_getattr,
//  .readlink = ramcloud_fuse_readlink,
  .mknod = fe_mknod,
//  .mkdir = ramcloud_fuse_mkdir,
//  .unlink = ramcloud_fuse_unlink,
//  .rmdir = ramcloud_fuse_rmdir,
//  .symlink = ramcloud_fuse_symlink,
//  .rename = ramcloud_fuse_rename,
//  .link = ramcloud_fuse_link,
//  .chmod = ramcloud_fuse_chmod,
//  .chown = ramcloud_fuse_chown,
//  .truncate = ramcloud_fuse_truncate,
//  //.utimens = ramcloud_fuse_utimens,
  .open = fe_open,
  .read = fe_read,
  .write = fe_write,
//  .statfs = ramcloud_fuse_statfs,
//  .flush = ramcloud_fuse_flush,
//  .release = ramcloud_fuse_release,
//  .fsync = ramcloud_fuse_fsync,
//  .opendir = fe_opendir,
  .readdir = fe_readdir,
//  .releasedir = ramcloud_fuse_releasedir,
//  .fsyncdir = ramcloud_fuse_fsyncdir,
//  .init = ramcloud_fuse_init,
//  .destroy = ramcloud_fuse_destroy,
//  .access = ramcloud_fuse_access,
//  .create = ramcloud_fuse_create,
};

static void the_fs_init(){
  tree_t *root;
  fe_data root_data;

  root_data.size = 1L;
  root_data.table_id = 1L;
  initstat(&root_data.vstat, S_IFDIR | 0755);

  root = tree_init();
  root->data = fe_data_to_void_ptr(root_data);

  the_fs.root = root;
}

int main(int argc, char **argv){
  int fuse_stat;

  the_fs_init();
  return fuse_main(argc, argv, &ramcloud_fuse_oper, NULL);
}




