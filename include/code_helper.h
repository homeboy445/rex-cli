#include "utility.h"

#include <iostream>
#include <vector>

class code_helper {
  std::string path;
  std::vector<std::string> code;

public:
  code_helper(std::string const &path);
  void find();
  void replace();
  void revert();
};