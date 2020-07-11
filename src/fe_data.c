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
  memcpy(&entry, p, sizeof(fe_data));
  return entry;
}

void *fe_data_to_void_ptr(fe_data d){
  void *mem = malloc(sizeof(fe_data));
  memcpy(mem, &d, sizeof(fe_data));
  return mem;
}
