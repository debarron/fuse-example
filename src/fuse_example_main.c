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

#include "fuse_example_fs.h"
#include "cpp_example.h"

#define MAX_DATA 256
#define STACK_MAX 256
char stack_data [STACK_MAX][MAX_DATA];
int stack_top = -1;
mode_t dir_mode, file_mode;

/* Result 1: OK
// Result 0: Error 
int stack_push(char *data){

  if (top == STACK_MAX-1)
    return 0;

  stack_top++;
  memset(stack_data[top], '\0', MAX_DATA);
  memcpy(data, stack_data[top], strlen(data));
  return 1;
}

char *stack_pop(){
  if(top == -1)
    return NULL;

  char *data = malloc(MAX_DATA);
  memset(data);
  memcpy(stack_data[stack_top], data);
  stack_top--;
  return data;
}
*/

void nf_add_inital_dirs(){
  nf_fs_dir_add("a");
  nf_fs_dir_add("b");
  nf_fs_dir_add("c");
}

void nf_set_stat(struct stat *vstat, int is_dir){
  time_t now = time(0);
  
  vstat->st_atime = now;
  vstat->st_ctime = now;
  vstat->st_mtime = now;
  vstat->st_mode = (is_dir) ? S_IFDIR : S_IFREG;
}


void nf_init(){
  cpp_print("This is from C");
  nf_fs_init();

  nf_add_inital_dirs();
}

void nf_dir_file(const char *path, char **dir, char **file){
  char *path_dir, *path_file;

  path_dir = dirname(path);
  path_file = basename(path);

  if(strcmp(path_dir, "/") == 0){
    *dir = strdup(path_file);
    *file = NULL;
  } else{
    *dir = strdup(&path_dir[1]);
    *file = strdup(path_file);
  }
}


static int nf_getattr(const char *path, struct stat *stbuf) {
  char *dir, *file;

  fprintf(stdout, "NF >> GETATTR Accessing %s\n", path);

  memset(stbuf, 0, sizeof(struct stat));
  nf_dir_file(path, &dir, &file);

  if(!nf_fs_file_exists(dir, file))
    return -errno;


  if (file == NULL){
    stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
    fprintf(stdout, "\tNF >> DIR %s\n", dir);
  } else{
    stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = 100;
    fprintf(stdout, "\tNF >> DIR %s FILE %s\n", dir, file);
  }

  //char *content = nf_fs_find(path);
  //if(content != NULL)
    //fprintf(stdout, ">> NF, From memory %s\n", content);

/*
  stbuf->st_mode   = node->vstat.st_mode;
  stbuf->st_nlink  = node->vstat.st_nlink;
  stbuf->st_size   = node->vstat.st_size;
  stbuf->st_blocks = node->vstat.st_blocks;
  stbuf->st_uid    = node->vstat.st_uid;
  stbuf->st_gid    = node->vstat.st_gid;
  stbuf->st_mtime  = node->vstat.st_mtime;
  stbuf->st_atime  = node->vstat.st_atime;
  stbuf->st_ctime  = node->vstat.st_ctime;

  // Directories contain the implicit hardlink '.'
  if(S_ISDIR(node->vstat.st_mode)) {
    stbuf->st_nlink++;
  }
*/

  return 0;
}

static int nf_read(
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

static int nf_opendir(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, "NF >> OPENDIR path %s\n", path);
  return status;
}

static int nf_readdir(
  const char *path,
  void *buffer,
  fuse_fill_dir_t filler,
  off_t offset,
  struct fuse_file_info *fi,
  enum fuse_readdir_flags flags)
{
  int status = 0;
  char *dir, *file, **entries;
  int n_entries;

  fprintf(stdout, "NF >> READDIR path %s\n", path);
  nf_dir_file(path, &dir, &file);

  entries = (dir[0] == '/') 
    ? nf_fs_list_root(&n_entries) 
    : nf_fs_list_dir(dir, &n_entries);
  
  fprintf(stdout, "IT GOT %d entries\n", n_entries);
  filler(buffer, ".", NULL, 0, 0);
  filler(buffer, "..", NULL, 0, 0);
  fprintf(stdout, ">> Added . .. to buffer\n");

  // print what we got
  for(int i = 0; i < n_entries; i++){
    fprintf(stdout, ">> %s\n", entries[i]);
  }

  // Create the stats
  struct stat *vstat = (struct stat *) malloc(sizeof(struct stat) * n_entries);
  for(int i = 0; i < n_entries; i++){
    nf_set_stat(&vstat[i], 1);
    filler(buffer, entries[i], &vstat[i], 0, 0);
  }


  // append all elements to filler
  // filler(.., NULL, 0)
  // filler(entryInDir, NULL, 0)
  return status;
}



int nf_mkdir(
  const char *path,
  mode_t mode)
{
  char *dir, *file;
  int exists;
  int status = -errno;

  nf_dir_file(path, &dir, &file);
  fprintf(stdout, "NF >> MKDIR Creating path=%s dir=%s file=%s\n", path, dir, file);

  exists = nf_fs_dir_add(file);
  dir_mode = mode;
  status = (exists) ? status : 0;

  return status;
}




/*
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

int ramcloud_fuse_open(
  const char *path,
  struct fuse_file_info *fi)
{
  int status = 0;
  fprintf(stdout, ">> OPEN Opening a file, path=%s\n", path);

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

*/

static struct fuse_operations ramcloud_fuse_oper = {
  .getattr = nf_getattr,
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
//  .open = ramcloud_fuse_open,
  .read = nf_read,
//  .write = ramcloud_fuse_write,
//  .statfs = ramcloud_fuse_statfs,
//  .flush = ramcloud_fuse_flush,
//  .release = ramcloud_fuse_release,
//  .fsync = ramcloud_fuse_fsync,
  .opendir = nf_opendir,
  .readdir = nf_readdir
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
