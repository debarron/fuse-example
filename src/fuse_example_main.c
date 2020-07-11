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

#include "fuse_file_operations.h"


static struct fuse_operations ramcloud_fuse_oper = {
  .getattr = fe_getattr,
//  .readlink = ramcloud_fuse_readlink,
//  .mknod = ramcloud_fuse_mknod,
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
//  .opendir = nf_opendir,
//  .readdir = nf_readdir
//  .releasedir = ramcloud_fuse_releasedir,
//  .fsyncdir = ramcloud_fuse_fsyncdir,
//  .init = ramcloud_fuse_init,
//  .destroy = ramcloud_fuse_destroy,
//  .access = ramcloud_fuse_access,
//  .create = ramcloud_fuse_create,
};

int main(int argc, char **argv){
  int fuse_stat;

  nf_init();
  return fuse_main(argc, argv, &ramcloud_fuse_oper, NULL);

}
