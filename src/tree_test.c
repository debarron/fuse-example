/*
 * =====================================================================================
 *
 *       Filename:  tree_test.c
 *
 *    Description: A simple example of tree 
 *
 *        Version:  1.0
 *        Created:  07/08/2020 16:36:19
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

int main(){
  tree_t *my_root = tree_init();
  tree_add(my_root, "/a", NULL);
  tree_add(my_root, "/b", NULL);
  tree_add(my_root, "/c", NULL);

  tree_add(my_root, "/a/a1", NULL);
  tree_add(my_root, "/a/a2", NULL);
  tree_add(my_root, "/a/a3", NULL);
  tree_add(my_root, "/c/c1", NULL);
  tree_add(my_root, "/c/c2", NULL);
  tree_add(my_root, "/c/c3", NULL);

  fprintf(stdout, "NODES ADDED \n");

  char *m = str_concat("A string", " concat");
  fprintf(stdout, "%s\n", m);
  //tree_print_children(my_root);
  tree_print_struct(my_root, " ");

  int path_lenght = 0;
  char *str = "/this/is/a/path";
  char **path_str = str_path_to_arr(str, &path_lenght);
  for(int i = 0; i < path_lenght; i++)
    fprintf(stdout, "%s\n", path_str[i]);



  return 0;
}

