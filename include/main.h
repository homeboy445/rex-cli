#include "code_helper.h"
#include <iostream>
#include <vector>

enum class operation_type { find, replace, revert };
enum class operation_scope { all, some };
enum class operation_target { includeOnly, excludeOnly };

class operation {
  std::string path;
  operation_type optype;
  std::string target;
  std::string replacer;
  operation_scope opscope;
  std::string rest;
  std::vector<string> filepaths;
  vector<pair<string, operation_target>> special_files;

public:
  operation(){};
  operation(std::string &p, std::string &tar, operation_type &o1,
            operation_scope &o2, std::string rt)
      : path(p), target(tar), optype(o1), opscope(o2), rest(rt) {}
  void reinitialize(
      std::string const &path, std::string const &target,
      operation_type const &optype, operation_scope const &opscope,
      std::string const rest, std::string const &replacer = "",
      std::vector<string> const filepaths = {},
      vector<pair<string, operation_target>> const &special_files = {}) {
    this->path = path, this->target = target, this->optype = optype,
    this->opscope = opscope, this->rest = rest, this->replacer = replacer,
    this->filepaths = filepaths, this->special_files = special_files;
  }
  void show() { // For debugging purposes only.
    cout << path << '\n'
         << "tar " << target << "\n"
         << "optype " << (optype == operation_type::find ? "find" : "replace")
         << "\n"
         << "opscope " << (opscope == operation_scope::all ? "all" : "some")
         << "\n"
         << "rep " << replacer << "\n"
         << "rest " << rest << "\n";
    cout << "filepaths : \n";
    for (auto const &p : filepaths) {
      cout << p << " ";
    }
    cout << "\nspecial files: ";
    for (auto const &p : special_files) {
      cout << "[ " << p.first << " "
           << (p.second == operation_target::includeOnly ? "includeOnly"
                                                         : "excludeOnly")
           << " ] ";
    }
  }
};

class REX {
  std::string code;
  std::vector<code_helper> instances;
  operation cur_op;

public:
  REX(char *arguments[], int const &size = 0);
  static void printUsage();
  void _parse(char *arguments[], int const &size = 0);
  void _register(std::string const &filepath);
  void _find(operation_type const &type);
  void _replace(operation_type const &type, std::string const &target);
  void _revert(std::string const &unique_code);
};
