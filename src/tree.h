#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t {
  char *path;
  void *data;
  struct node_t *children;
  struct node_t *next;
  int children_n;
} tree_t;

char *str_duplicate(char *value);
char *str_concat(char *str1, char *str2);
int str_count_occurrence(char *str, char c);
char **str_path_to_arr(char *path, int *array_n);
char *str_array_to_str(char **arr, int arr_lenght);


tree_t *tree_new_node(char *path, void *data);
tree_t* tree_init();

void tree_add_children(tree_t *root, char *element, void *data);
char tree_add(tree_t *root, char *path, void *data);

void tree_print_children(tree_t *root);

void tree_print_struct(tree_t *root, char *suffix);

tree_t *tree_find_next(tree_t *root, char *element);
tree_t *tree_find_children(tree_t *root, char *element);
tree_t *tree_find(tree_t *root, char *path);


#endif
