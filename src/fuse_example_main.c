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

  fprintf(stdout, "\t >> getattr found path='%s'\n", entry->path);
  entry_info = fe_data_from_void_ptr(entry->data);
  stbuf->st_mode   = entry_info.vstat.st_mode;
  stbuf->st_nlink  = entry_info.vstat.st_nlink;
  stbuf->st_size   = entry_info.vstat.st_size;
  stbuf->st_blocks = entry_info.vstat.st_blocks;
  stbuf->st_uid    = entry_info.vstat.st_uid;
  stbuf->st_gid    = entry_info.vstat.st_gid;
  stbuf->st_mtime  = entry_info.vstat.st_mtime;
  stbuf->st_atime  = entry_info.vstat.st_atime;
  stbuf->st_ctime  = entry_info.vstat.st_ctime;

  // Directories contain the implicit hardlink '.'
  if(S_ISDIR(entry_info.vstat.st_mode)) {
    stbuf->st_nlink++;
  }

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
  tree_t *entry;
  fe_data entry_data;
  void *old_entry_data;
  void *new_data;
  int new_data_lenght;

  fprintf(stdout, ">> FUNCTION: fe_write path='%s'\n", path);
  fprintf(stdout, "\t>> fe_write buf='%s'\n", buf);

  new_data_lenght = offset + size;
  new_data = malloc(new_data_lenght);
  memset(new_data, 0, new_data_lenght);

  entry = tree_find(the_fs.root, path);
  entry_data = fe_data_from_void_ptr(entry->data);
  if(entry_data.content_size > 0)
    memcpy(new_data, entry_data.content, entry_data.content_size);

  memcpy(((char *)new_data) + offset, buf, size);
  old_entry_data = entry->data;
  entry_data.content_size = new_data_lenght;
  entry_data.content = (char *) new_data;
  entry_data.vstat.st_size = size;
  entry->data = fe_data_to_void_ptr(entry_data);

  free(old_entry_data);
  return size;
}


static int fe_read(
  const char *path,
  char *buf,
  size_t size,
  off_t offset,
  struct fuse_file_info *fi)
{
  tree_t *entry;
  fe_data entry_data;
  void *old_entry_data;
  
  fprintf(stdout, ">> FUNCTION: fe_read path='%s'\n", path);

  entry = tree_find(the_fs.root, path);
  entry_data = fe_data_from_void_ptr(entry->data);

  int has_reach_end = (offset >= size) ? 1 : 0;
  int entry_not_exists = (entry == NULL) ? 1 : 0;

  if(has_reach_end || entry_not_exists) return 0;

  fprintf(stdout, "\t>> fe_read size='%d' offset='%d'\n", size, offset);
  fprintf(stdout, "\t>> fe_read content='%s' content_size='%d'\n", 
      entry_data.content, entry_data.content_size);

  // Calculate number of bytes to copy
  size_t avail = entry_data.vstat.st_size - offset;
  size_t n = (size < avail) ? size : avail;
  memcpy(buf, entry_data.content + offset, n);

  return n;
}

static int fe_mknod(
    const char *path, 
    mode_t mode, 
    dev_t rdev) 
{
  fe_data new_entry;

  fprintf(stdout, ">> FUNCTION fe_mknod path='%s'\n", path);

  new_entry.content_size = 0;
  new_entry.vstat.st_size = 0;
  new_entry.content = NULL;
  initstat(&new_entry.vstat, mode);
  tree_add(the_fs.root, path, fe_data_to_void_ptr(new_entry));
  
  return 0;
}

static void fe_readdir_fill_up(
  tree_t *parent,
  void *buffer,
  fuse_fill_dir_t filler)
{
  tree_t *child = parent->children;
  fe_data child_data;

  while(child != NULL){
    child_data = fe_data_from_void_ptr(child->data);
    filler(buffer, child->path, &child_data.vstat, 0, 0);
    child = child->next;
  }
}

static int fe_mkdir(
  const char *path, 
  mode_t mode) 
{
  fe_data new_dir_info;

  fprintf(stdout, ">> FUNCTION fe_mkdir path='%s'\n", path);

  new_dir_info.content_size = 0;
  new_dir_info.content = NULL;
  initstat(&new_dir_info.vstat, S_IFDIR | mode);
  tree_add(the_fs.root, path, fe_data_to_void_ptr(new_dir_info));
  
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
  tree_t *entry, *parent;
  fe_data entry_data, parent_data;
  int entry_exists, entry_is_file;
  struct stat *parent_vstat;
  
  fprintf(stdout, ">> FUNCTION: fe_readdir path='%s'\n", path);

  entry = tree_find(the_fs.root, path);
  entry_data = fe_data_from_void_ptr(entry->data);
  entry_exists = (entry != NULL) ? 1 : 0;
  entry_is_file = (S_ISREG(entry_data.vstat.st_mode)) ? 1 : 0;

  if(!entry_exists || entry_is_file){
    errno = ENOENT;
    return -errno;
  } 

  filler(buffer, ".",  &entry_data.vstat, 0, 0);
  if(strcmp(path, THE_ROOT) == 0) 
    parent_vstat = NULL;
  else{
    parent = tree_find_parent(the_fs.root, path);
    parent_data = fe_data_from_void_ptr(parent->data);
    parent_vstat = &parent_data.vstat;
  }

  fprintf(stdout, "\t>> fe_readdir passed the parent test\n");
  filler(buffer, "..", parent_vstat, 0, 0);
  fe_readdir_fill_up(entry, buffer, filler);

  return 0;
}


static struct fuse_operations ramcloud_fuse_oper = {
  .getattr = fe_getattr,
//  .readlink = ramcloud_fuse_readlink,
  .mknod = fe_mknod,
  .mkdir = fe_mkdir,
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




