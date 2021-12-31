#include "utility.h"

#include <iostream>
#include <vector>

class code_helper {
  std::string path;
  std::vector<std::string> code;

public:
  code_helper(std::string const &path);
  std::string &getpath() { return this->path; }
  std::vector<std::string> &getcode() { return this->code; }
  std::vector<std::pair<int, std::string>> find(std::string const &target);
  void replace();
  void revert();
};