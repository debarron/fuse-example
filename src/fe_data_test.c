/*
 * =====================================================================================
 *
 *       Filename:  fe_data_test.c
 *
 *    Description: Test example on how to use fe_data structure 
 *
 *        Version:  1.0
 *        Created:  07/10/2020 13:58:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Lopez
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "fe_data.h"


int main(){
  fe_data my_data, my_data_from_file;
  void *my_data_ptr;
  FILE *file;

  char *msg = "hello world\0";
  file = fopen("./fe_data_file.bin", "wb");

  // do something with my_data
  my_data.table_id = 1L;
  my_data.vstat.st_mode = S_IFDIR;
  my_data.content_size = strlen(msg);
  my_data.content = (char *) malloc(sizeof(char) * strlen(msg));
  memcpy(my_data.content, msg, strlen(msg));

  // pass my_data to my_data_ptr
  my_data_ptr = fe_data_to_void_ptr(my_data);

  // write my_data_ptr
  fwrite(my_data_ptr, sizeof(fe_data) + my_data.content_size, 1, file);
  fclose(file);

  // read my_data_ptr file to my_data_from_file
  file = fopen("./fe_data_file.bin", "rb");
  my_data_ptr = malloc(sizeof(fe_data) + my_data.content_size);
  fread(my_data_ptr, sizeof(fe_data) + my_data.content_size, 1, file);
  fclose(file);
  my_data_from_file = fe_data_from_void_ptr(my_data_ptr);

  // compare my_data vs. my_data_ptr_from_file
  fprintf(stdout, "%s == %s ? %d\n", my_data.content, my_data_from_file.content, 
      strcmp(my_data.content, my_data_from_file.content));
  fprintf(stdout, "%d == %d ? %d\n", my_data.size, my_data_from_file.size, 
      (my_data.size == my_data_from_file.size));
  fprintf(stdout, "%d == %d ? %d\n", my_data.table_id, my_data_from_file.table_id, 
      (my_data.table_id == my_data_from_file.table_id));
  fprintf(stdout, "%d == %d ? %d\n", my_data.vstat.st_mode, my_data_from_file.vstat.st_mode, 
      (my_data.vstat.st_mode == my_data_from_file.vstat.st_mode));
}
