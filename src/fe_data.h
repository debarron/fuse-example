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

#ifndef FE_DATA_H
#define FE_DATA_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct{
  long size;
  struct stat vstat;
  long table_id;
}fe_data;

fe_data fe_data_from_void_ptr(void *p);

void *fe_data_to_void_ptr(fe_data d);

#endif
