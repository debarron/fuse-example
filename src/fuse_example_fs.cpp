#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <vector>

#include "fuse_example_fs.h"

using namespace std;

map<string, vector<string>> my_fs;
map<string, string>::iterator it;

char *copy_str(string value){
  char *mem = new char[value.size() +1];
  copy(value.begin(), value.end(), mem);
  mem[value.size()] = '\0';
  return mem;
}


void nf_fs_init(){
  my_fs.clear();
  vector<string> rootEntry;
  rootEntry.push_back(string("THIS IS THE ROOT DIR"));
  my_fs["/"] = rootEntry;
}

int nf_fs_dir_exists(const char *dir){
  string key = string(dir);
  auto dir_element = my_fs.find(key);

  else if (dir_element == my_fs.end()) return 0;
    
  return 1;
}

int nf_fs_file_exists(const char *dir, const char *file){
  string key = string(dir);
  string file_name = string(file);
  vector<string> entries;
  vector<string>::iterator it;
  int file_exists = 0

  if(!nf_fs_dir_exists(dir)) return file_exists;

  entries = my_fs[key];
  it = my_fs.begin();
  while(!file_exists && it != my_fs.end()){
    if(file_name.compare(it) == 0)
      file_exists = 1;

    ++it;
  }

  return file_exists;
}


int nf_fs_dir_add(const char *dir){
  string new_dir; 
  vector<string> empty_vector;

  if(nf_fs_dir_exists(dir)) return 0;

  new_dir = string(dir);
  my_fs[new_dir] = empty_vector;
  return 1;
}

int nf_fs_file_add(const char *dir, const char *file){
  string file_name, dir_name;
  vector<string> entries;

  if(!nf_fs_dir_exists(dir)) return 0;
  if(nf_fs_file_exists(dir, file)) return 0;

  dir_name = string(dir);
  file_name = string(file);
  my_fs[dir_name].push_back(file_name);

  return 1;
}

char **nf_fs_list_root(int *n_entries){
  char **entries;
  int i;
  map<string,vector<string>::iterator it;

  *n_entries = my_fs.size();
  entries = (char **)malloc(sizeof(char *) * *n_entries);
  i = 0;
  for (it = my_fs.begin(); it != my_fs.end(); ++it)
    entries[i] = copy_str(it->first);
  
  return entries;
}

char **nf_fs_list_dir(const char *dir, int *n_entries){
  char **dir_entries;
  string key;
  vector<string> entries;

  *n_entries = 0;
  if(!nf_fs_dir_exists(dir))
    return NULL;
  
  key = string(dir);
  if(key.compare("/") == 0)
    return nf_fs_list_root(n_entries);

  entries = my_fs[key];
  *n_entries = entries.size();
  dir_entries = (char **)malloc(sizeof(char *) * *n_entries);

  for (int i = 0; i < entries.size(); i++)
    dir_entries[i] = copy_str(entries[i]);

  return dir_entries;
}


