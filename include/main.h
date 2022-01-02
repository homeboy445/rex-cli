#include "code_helper.h"
#include <chrono>
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
  int lines;
  std::string rest;
  vector<pair<string, operation_target>> special_files;

public:
  operation(){};
  operation(std::string &p, std::string &tar, operation_type &o1,
            operation_scope &o2, std::string rt)
      : path(p), target(tar), optype(o1), opscope(o2), rest(rt) {}
  void reinitialize(
      std::string const &path, std::string const &target,
      operation_type const &optype, operation_scope const &opscope,
      int const &lines, std::string const rest,
      std::string const &replacer = "",
      vector<pair<string, operation_target>> const &special_files = {}) {
    this->path = path, this->target = target, this->optype = optype,
    this->opscope = opscope, this->lines = lines, this->rest = rest,
    this->replacer = replacer, this->special_files = special_files;
  }
  operation_type const &get_operationtype() { return this->optype; }
  operation_scope const &get_operationscope() { return this->opscope; }
  std::pair<std::string, std::string> const get_targetAndreplacer() {
    return {this->target, this->replacer};
  }
  vector<pair<string, operation_target>> const &get_specialfiles() {
    return this->special_files;
  }
  int const &getlines() { return this->lines; };
  void show() { // For debugging purposes only.
    cout << path << '\n'
         << "target " << target << "\n"
         << "optype " << (optype == operation_type::find ? "find" : "replace")
         << "\n"
         << "opscope " << (opscope == operation_scope::all ? "all" : "some")
         << "\n"
         << "replace_target " << replacer << "\n"
         << "lines " << lines << "\n"
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
  std::string unique_identifier; // This is just basically a concatinated
                                 // version of the command invoked.
  std::string code;
  std::vector<code_helper> file_instances;
  operation cur_op;
  std::unordered_map<std::string, bool> filepaths;

  std::unordered_map<
      std::string,
      std::vector<std::pair<std::string, std::pair<int, std::string>>>>
      cached_results; /*This basically is responsible for checking if a command
                         has been repeated or not, if it has been then return
                         the cached version, if not, then compute it.*/
  std::vector<std::pair<std::string, std::pair<int, std::string>>>
      found_results;

  // For measuring performance
  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;

public:
  REX(char *arguments[], int const &size = 0);
  static void printUsage();
  int getElapsedTime();
  void _parse(char *arguments[], int const &size = 0);
  void _register(std::string const &filepath);
  void _initiate_operation();
  void _find();
  void _replace();
  void _revert();
};
