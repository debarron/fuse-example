#ifndef FUSE_EXAMPLE_FS
#define FUSE_EXAMPLE_FS

#ifdef __cplusplus
extern "C" {
#endif
void nf_fs_init();

void nf_fs_add(const char *entry);

char *nf_fs_find(const char *key);

char **nf_fs_list();

#ifdef __cplusplus
}
#endif

#endif
