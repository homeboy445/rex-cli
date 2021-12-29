#include "../include/code_helper.h"

#include <fstream>

using namespace std;

code_helper::code_helper(string const &path) {
  this->path = path;
  fstream file(path, ios::in);
  string data, tmp;
  while (getline(file, tmp))
    data += tmp;
  file.close();
  this->code = utility::split(data);
}
