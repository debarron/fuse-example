#ifndef FUSE_EXAMPLE_FS
#define FUSE_EXAMPLE_FS

#ifdef __cplusplus
extern "C" {
#endif
void nf_fs_init();

int nf_fs_dir_exists(const char *dir);
int nf_fs_file_exists(const char *dir, const char *file);

int nf_fs_dir_add(const char *dir);
int nf_fs_file_add(const char *dir, const char *file);

char **nf_fs_list_root(int *n_entries);
char **nf_fs_list_dir(const char *dir, int *n_entries);

#ifdef __cplusplus
}
#endif

#endif
