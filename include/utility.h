#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace std::filesystem;

class utility {
public:
  static std::string getcwd_string() {
    char buff[int(1e4)];
    getcwd(buff, int(1e4));
    std::string cwd(buff);
    return cwd;
  }
  static std::string validate_path(std::string const &path) {
    if (path[0] != '.') {
      return path;
    }
    return getcwd_string() + path.substr(1, path.length() - 1);
  }
  static std::string getsubstr(std::string const &target, int start, int end) {
    return std::string(target.begin() + start, target.begin() + end);
  }
  static std::string getfiletype(std::string const &file) {
    int x = file.find_last_of('.');
    if (x == -1)
      return "invalid!";
    return getsubstr(file, x + 1, file.length());
  }
  static void trim(std::string &target) {
    std::string s;
    int start = 0, end = target.length() - 1;
    while (target[start++] == ' ' && start < target.length())
      ;
    while (target[end--] == ' ' && end >= 0)
      ;
    for (int i = start - 1; i <= end + 1; s.push_back(target[i++]))
      ;
    target = s;
  }
  static bool isfiletypevalid(std::string const &type) {
    for (auto const &t : type) {
      if (!isalpha(t))
        return false;
    }
    return true;
  }
  /**
   * @brief A utility method for spliting the code with respect to a specified
   * seperator(which is '\n' by default.) and returning a vector of strings.
   *
   * @param target
   * @param seperator
   * @return std::vector of std::string
   */
  static std::vector<std::string> split(std::string const &target,
                                        char const &seperator = '\n') {
    std::string cur_code = "";
    std::vector<string> results;
    for (auto &ch : target) {
      if (ch == seperator) {
        results.push_back(cur_code);
        cur_code.clear();
      } else {
        cur_code.push_back(ch);
      }
    }
    return results;
  }
  static std::string colorise_occurences(std::string const &main,
                                         std::string const &target) {
    string main_ = "", s = "";
    if (target.find('\n') !=
        std::string::npos) { // Add support for multiword coloring...
      return main;
    }
    for (int i = 0; i < main.length(); i++) {
      for (int j = i; j < i + target.length() && j < main.length(); j++) {
        s.push_back(main[j]);
      }
      if (s.compare(target) == 0) {
        main_ += "\033[1;31m" + target + "\033[0m";
        i += target.length() - 1;
      } else {
        main_.push_back(main[i]);
      }
      s.clear();
    }
    return main_;
  }
  static void save_log(std::string const &data, std::string const &target,
                       std::string const &replacer) {
    auto now = std::chrono::system_clock::now();
    auto now_ = std::chrono::system_clock::to_time_t(now);
    string f_name = ctime(&now_);
    for (auto &ch : f_name) {
      if (ch == ' ')
        ch = '_';
    }
    path f1(f_name);
    path f2(utility::getcwd_string());
    path k = f2 / f1;
    f_name = k;
    f_name.back() = '_';
    f_name += target + "_" + replacer + ".log";
    ofstream file(f_name);
    file << data;
    file.close();
    cout << "The data has been successfully stored in the log file at "
            "location(please don't tamper with it): "
         << f_name << " \n";
  }
};
