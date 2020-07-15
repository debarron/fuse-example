/*
 * =====================================================================================
 *
 *       Filename:  fe_data.c
 *
 *    Description: A data structure to keep the FUSE info coupled 
 *
 *        Version:  1.0
 *        Created:  07/10/2020 13:45:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Lopez 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "fe_data.h"

fe_data fe_data_from_void_ptr(void *p){
  fe_data entry;
  char *buffer_wrapper;
 
  buffer_wrapper = (char *) p;
  memcpy(&entry, buffer_wrapper, sizeof(fe_data));
  entry.content = (char *) malloc(sizeof(char) * entry.content_size);

  memset(entry.content, 0, sizeof(char) * entry.content_size);
  memcpy(entry.content, &buffer_wrapper[sizeof(fe_data)], entry.content_size);
  return entry;
}

void *fe_data_to_void_ptr(fe_data d){
  void *buffer;
  char *buffer_wrapper;

  buffer = malloc(sizeof(fe_data) + d.content_size);
  buffer_wrapper = (char *) buffer;

  memset(buffer_wrapper, 0, sizeof(fe_data) + d.content_size);
  memcpy(buffer_wrapper, &d, sizeof(fe_data));
  memcpy(&buffer_wrapper[sizeof(fe_data)], d.content, d.content_size);

  return buffer;
}
