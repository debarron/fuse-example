
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

char *str_duplicate(char *value){
  int lenght = strlen(value);

  char *new_str = (char *) malloc(sizeof(char) * lenght + 1);
  strcpy(new_str, value);
  new_str[lenght] = '\0';
  return new_str;
}

char *str_concat(char *str1, char *str2){
  int lenght = strlen(str1) + strlen(str2);
  char *new_str = (char *) malloc(sizeof(char) * (lenght + 1));

  sprintf(new_str, "%s%s%s", str1, str2, "\0");
  return new_str;
}

int str_count_occurrence(char *str, char c){
  int occurrences = 0;

  for(int i = 0; str[i] != '\0'; i++)
    occurrences += (str[i] == c) ? 1 : 0;

  return occurrences;
}

char **str_path_to_arr(char *path, int *array_n){
  char **path_array = NULL;
  char *ptr_path = NULL;
  char *path_bak = str_duplicate(path);
  int lenght = strlen(path_bak);
  int i = 0;

  if(path_bak[lenght-1] == '/')
    path_bak[--lenght] = '\0';

  *array_n = str_count_occurrence(path_bak, '/') + 1; 
  path_array = (char **) malloc(sizeof(char *) * *array_n);
  path_array[i++] = str_duplicate("/");

  ptr_path = strtok(path_bak, "/");
  while(ptr_path != NULL){
    path_array[i++] = str_duplicate(ptr_path);
    ptr_path = strtok(NULL, "/");
  }

  free(path_bak);
  return path_array;
}

char *str_array_to_str(char **arr, int arr_lenght){
  int str_lenght, current_pos;
  char *str;

  str_lenght = 1;
  for(int i = 0; i < arr_lenght; i++)
    str_lenght += strlen(arr[i]);
  
  str_lenght += (arr_lenght > 2) ? arr_lenght - 2 : 0;
  str = (char *) malloc(sizeof(char) * str_lenght);

  current_pos = 0;
  for(int i = 0; i < arr_lenght; i++){
    if(i > 2) str[current_pos++] = '/';

    strcpy(&str[current_pos], arr[i]);
    current_pos += strlen(arr[i]);
  }
  str[current_pos] = '\0';
  return str;
}

tree_t *tree_new_node(char *path, void *data){
  tree_t *new_node = (tree_t *) malloc(sizeof(tree_t));
  
  if(new_node == NULL) return NULL;
  
  new_node->path = str_duplicate(path);
  new_node->data = data;
  new_node->children = NULL;
  new_node->next = NULL;
  new_node->children_n = 0;

  return new_node;
}

tree_t* tree_init(){
  return tree_new_node("/", NULL);
}

void tree_add_children(tree_t *root, char *element, void *data){
  tree_t *new_node = tree_new_node(element, data);
  tree_t *children = root->children;

  root->children_n++;
  if(root->children == NULL)
    root->children = new_node;
  else{
    while(children->next != NULL)
      children = children->next;

    children->next = new_node;
  }
}

void tree_print_children(tree_t *root){
  tree_t *child = root->children;
  while(child != NULL){
    fprintf(stdout, "%s\n", child->path);
    child = child->next;
  }
}

void tree_print_struct(tree_t *root, char *suffix){
  tree_t *children;

  if(root == NULL) return;
  else{
    fprintf(stdout, "--%s\n", str_concat(suffix, root->path));
    tree_print_struct(root->children, str_concat(suffix, " +"));
    tree_print_struct(root->next, suffix);
  }
}

tree_t *tree_find_next(tree_t *root, char *element){
  tree_t *result = NULL;
  char found = 0;

  for(tree_t *current = root; (current != NULL && !found); current = current->next)
    if(strcmp(current->path, element) == 0){
      result = current;
      found = 1;
    }

  return result;
}

tree_t *tree_find_children(tree_t *root, char *element){
  if(root->children == NULL) return NULL;
  else return tree_find_next(root->children, element);
}

char tree_add(tree_t *root, char *path, void *data){
  char **arr_path, *prev_path, *new_element;
  int arr_path_lenght;
  tree_t *parent;

  arr_path = str_path_to_arr(path, &arr_path_lenght);
  new_element = arr_path[arr_path_lenght-1];
  prev_path = str_array_to_str(arr_path, arr_path_lenght-1);
  
  parent = tree_find(root, prev_path);
  if(parent == NULL) return 0;

  tree_add_children(parent, new_element, data);
  return 1;
}

tree_t *tree_find(tree_t *root, char *path){
  char **path_arr;
  int path_arr_lenght;
  char not_exists = 0;
  tree_t *result = NULL;
  tree_t *temp = NULL;

  if(strcmp(root->path, path) == 0)
    return root;
  
  temp = root;
  path_arr = str_path_to_arr(path, &path_arr_lenght);
  for(int i = 1; i < path_arr_lenght && !not_exists; i++){
    result = tree_find_children(temp, path_arr[i]);

    if(result == NULL)
      not_exists = 1;
  
    temp = result;
  }

  return result;
}

