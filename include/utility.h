#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

class utility {
public:
  static std::string getcwd_string() {
    char buff[int(1e4)];
    getcwd(buff, int(1e4));
    std::string cwd(buff);
    return cwd;
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
  static bool isfiletypevalid(std::string const &type) {
    for (auto const &t : type) {
      if (!isalpha(t))
        return false;
    }
    return true;
  }
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
};
