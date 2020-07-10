/**
 * The following is a set of small functions to
 * handle strings in C. Simple and convenient.
 *
 * @author Daniel Lopez
 * @since 2020-07-10 
 */


#ifndef STR_FUNCTIONS_H
#define STR_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** 
 * This function duplicates a string
 * and adds a null-terminated char at the end.
 * @param value This is the string to be duplicated
 * @return char* The duplicated string 
 */
char *str_duplicate(char *value);

char *str_concat(char *str1, char *str2);

int str_count_occurrence(char *str, char c);

char **str_path_to_arr(char *path, int *array_n);

char *str_array_to_str(char **arr, int arr_lenght);

#endif
