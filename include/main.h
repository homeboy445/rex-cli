#include "code_helper.h"
#include <iostream>
#include <unordered_map>
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
  std::unordered_map<std::string, bool> filepaths;
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
      std::unordered_map<std::string, bool> const filepaths = {},
      vector<pair<string, operation_target>> const &special_files = {}) {
    this->path = path, this->target = target, this->optype = optype,
    this->opscope = opscope, this->rest = rest, this->replacer = replacer,
    this->filepaths = filepaths, this->special_files = special_files;
  }
  operation_type const &get_operationtype() { return this->optype; }
  operation_scope const &get_operationscope() { return this->opscope; }
  std::unordered_map<std::string, bool> const &get_filepaths() {
    return this->filepaths;
  };
  std::pair<std::string, std::string> const get_targetAndreplacer() {
    return {this->target, this->replacer};
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
  std::vector<code_helper> file_instances;
  operation cur_op;

  std::vector<std::pair<std::string, std::pair<int, pair<std::string, int>>>>
      found_results;

public:
  REX(char *arguments[], int const &size = 0);
  static void printUsage();
  void _parse(char *arguments[], int const &size = 0);
  void _register(std::string const &filepath);
  void _initiate_operation();
  void _find();
  void _replace();
  void _revert();
};
