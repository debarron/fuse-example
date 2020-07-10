/**
 * This is a set of string functions
 * used in different projects.
 * Makes it easier to handle strings 
 * in C.
 * @author Daniel Lopez
 * @since 2020-07-10
 */

#include "str_functions.h"

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

