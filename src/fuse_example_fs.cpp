#include <iostream>
#include <string>
#include <map>

#include "fuse_example_fs.h"

extern "C" {
#include <stdio.h>
#include <string.h>

using namespace std;
map<string, string> my_fs;
map<string, string>::iterator it;

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
    found = strdup(element->second.c_str());
  
  return found;
}

char **nf_fs_list(){
  char **entry_list; 
  entry_list = (char **)malloc(sizeof(char*) * my_fs.size());

  it = my_fs.begin();
  for(int i = 0; i < my_fs.size(); i++){
    entry_list[i] = strdup(it->second.c_str());
    it++;
  }

  return entry_list;
}

}
