#include <iostream>
#include <cstdio>
#include <string>
#include <map>

#include "fuse_example_fs.h"

using namespace std;
map<string, string> my_fs;
map<string, string>::iterator it;

char *copy_str(string value){
  char *mem = new char[value.size() +1];
  copy(value.begin(), value.end(), mem);
  mem[value.size()] = '\0';
  return mem;
}


void nf_fs_init(){
  my_fs.clear();
  my_fs["/"] = "First entry";
}

void nf_fs_add(const char *entry){
  string fs_entry = string(entry);
  my_fs[fs_entry] = "New entry added";
}

char *nf_fs_find(const char *key){
  string fs_key = string(key);
  char *found = NULL;

  auto element = my_fs.find(fs_key);
  if(element != my_fs.end())
    found = copy_str(element->second);

  return found;
}

char **nf_fs_list(){
  char **entry_list; 
  entry_list = (char **)malloc(sizeof(char*) * my_fs.size());

  it = my_fs.begin();
  for(int i = 0; i < my_fs.size(); i++){
    entry_list[i] = copy_str(it->second);
    it++;
  }

  return entry_list;
}


