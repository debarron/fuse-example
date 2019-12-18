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
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void *ramcloud_fuse_init(
  struct fuse_conn_info *conn,
  struct fuse_config *config)
{
  int status = 0;
  fprintf(stdout, ">> INIT Filesystem initiated/n");
  return NULL;
}


int ramcloud_fuse_getattr(
  const char *path,
  struct stat *st)
{
  int status = 0;

  fprintf(stdout, ">> GETATTR Getting file attributes, path=%s\n", path);

  st->st_uid = getuid();
  st->st_gid = getgid();
  st->st_atime = time(NULL);
  st->st_mtime = time(NULL);
  st->st_mode = 0644;
  st->st_nlink = 1;
  st->st_size = 128;

  fprintf(stdout, ">> End of GETATTR\n");
  return status;
}

int ramcloud_fuse_mkdir(
  const char *path,
  mode_t mode)
{
  int status = 0;
  fprintf(stdout, ">> MKDIR Creating a directory, path=%s\n", path);

  // TODO This calls ramcloud create table

  return status;
}

int ramcloud_fuse_open(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> OPEN Opening a file, path=%s\n", path);

  // TODO This calls ramcloud read

  return status;
}

int ramcloud_fuse_read(
  const char *path,
  char *buf,
  size_t size,
  off_t offset,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> READ Reading a file, path=%s\n", path);

  // TODO This calls ramcloud read

  return status;
}

int ramcloud_fuse_write(
  const char *path,
  const char *buf,
  size_t size,
  off_t offset,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> WRITE Writing a file, path=%s\n", path);

  // TODO This calls ramcloud read

  return status;
}

int ramcloud_fuse_statfs(
  const char *path,
  struct statvfs *statv)
{
  int status = 0;
  fprintf(stdout, ">> STATFS Stating a file, path=%s\n", path);

  // TODO This calls ramcloud read

  return status;
}

int ramcloud_fuse_readlink(
  const char *path,
  char *buffer,
  size_t size)
{
  int status = 0;
  fprintf(stdout, ">> READLINK path=%s", path);

  return status;
}

int ramcloud_fuse_mknod(
  const char *path,
  mode_t access,
  dev_t device
)
{
  int status = 0;
  fprintf(stdout, ">> MKNOD, path=%s\n", path);

  return status;
}

int ramcloud_fuse_unlink(
  const char *path
)
{
  int status = 0;
  fprintf(stdout, ">> UNLINK, path=%s\n", path);

  return status;
}

int ramcloud_fuse_rmdir(
  const char *path
)
{
  int status = 0;
  fprintf(stdout, ">> RMDIR, path=%s\n", path);

  return status;
}

int ramcloud_fuse_symlink(
  const char *path,
  const char *link)
{
  int state = 0;
  fprintf(stdout, ">> SYMLINK path=%s\n", path);
  return state;
}

int ramcloud_fuse_rename(
  const char *path,
  const char *new_path,
  unsigned int flags)
{
  int status = 0;
  fprintf(stdout, ">> RENAME path=%s\n", path);
  return status;
}

int ramcloud_fuse_link(
  const char *path,
  const char *link)
{
  int status = 0;
  fprintf(stdout, ">> LINK path=%s\n", path);
  return status;
}

int ramcloud_fuse_chmod(
  const char *path,
  mode_t object_mode,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> CHMOD path=%s\n", path);
  return status;
}

int ramcloud_fuse_chown(
  const char *path,
  uid_t user_id,
  gid_t group_id,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> CHOWN path=%s\n", path);
  return status;
}

int ramcloud_fuse_truncate(
  const char *path,
  off_t offset,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> TRUNCATE path=%s\n", path);
  return status;
}

int ramcloud_fuse_utimens(
  const char *path,
  const struct timespec ts[2],
  const char *link)
{
  int status = 0;
  fprintf(stdout, ">> SYMLINK path=%s\n", path);
  return status;
}

int ramcloud_fuse_flush(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> FLUSH path=%s\n", path);
  return status;
}

int ramcloud_fuse_release(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> RELEASE path=%s\n", path);
  return status;
}

int ramcloud_fuse_fsync(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> FSYNC path=%s\n", path);
  return status;
}

int ramcloud_fuse_opendir(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> OPENDIR path=%s\n", path);
  return status;
}

int ramcloud_fuse_readdir(
  const char *path,
  void *buffer,
  fuse_fill_dir_t filer,
  off_t offset,
  struct fuse_file_info *fi,
  enum fuse_readdir_flags flags)
{
  int status = 0;
  fprintf(stdout, ">> READDIR path=%s\n", path);
  return status;
}

int ramcloud_fuse_releasedir(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> RELEASEDIR path=%s\n", path);
  return status;
}

int ramcloud_fuse_fsyncdir(
  const char *path,
  int fsync_count,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> FSYNCDIR path=%s\n", path);
  return status;
}

void *ramcloud_fuse_destroy(
  void *private_data)
{
  int status = 0;
  fprintf(stdout, ">> DESTROY \n");
  return NULL;
}

int ramcloud_fuse_access(
  const char *path,
  int mask)
{
  int status = 0;
  fprintf(stdout, ">> ACCESS path=%s\n", path);
  return status;
}

int ramcloud_fuse_create(
  const char *path,
  mode_t access_mode,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> CREATE path=%s\n", path);
  return status;
}

static struct fuse_operations ramcloud_fuse_oper = {
  .getattr = ramcloud_fuse_getattr,
  .readlink = ramcloud_fuse_readlink,
  .mknod = ramcloud_fuse_mknod,
  .mkdir = ramcloud_fuse_mkdir,
  .unlink = ramcloud_fuse_unlink,
  .rmdir = ramcloud_fuse_rmdir,
  .symlink = ramcloud_fuse_symlink,
  .rename = ramcloud_fuse_rename,
  .link = ramcloud_fuse_link,
  .chmod = ramcloud_fuse_chmod,
  .chown = ramcloud_fuse_chown,
  .truncate = ramcloud_fuse_truncate,
  //.utimens = ramcloud_fuse_utimens,
  .open = ramcloud_fuse_open,
  .read = ramcloud_fuse_read,
  .write = ramcloud_fuse_write,
  .statfs = ramcloud_fuse_statfs,
  .flush = ramcloud_fuse_flush,
  .release = ramcloud_fuse_release,
  .fsync = ramcloud_fuse_fsync,
  .opendir = ramcloud_fuse_opendir,
  .readdir = ramcloud_fuse_readdir,
  .releasedir = ramcloud_fuse_releasedir,
  .fsyncdir = ramcloud_fuse_fsyncdir,
  .init = ramcloud_fuse_init,
  .destroy = ramcloud_fuse_destroy,
  .access = ramcloud_fuse_access,
  .create = ramcloud_fuse_create,
};

int main(int argc, char **argv){
  int fuse_stat;

  return fuse_main(argc, argv, &ramcloud_fuse_oper, NULL);

}
