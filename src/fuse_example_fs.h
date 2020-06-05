#ifndef FUSE_EXAMPLE_FS
#define FUSE_EXAMPLE_FS

extern "C" {
void nf_fs_init();

void nf_fs_add(const char *entry);

char *nf_fs_find(const char *key);

char **nf_fs_list();
}
#endif
