#include <iostream>
#include <unistd.h>
#include <vector>

using namespace std;

class utility {
public:
  static string getcwd_string() {
    char buff[int(1e5)];
    getcwd(buff, int(1e5));
    std::string cwd(buff);
    return cwd;
  }
  static string getsubstr(std::string const &target, int start, int end) {
    return string(target.begin() + start, target.end() + end + 1);
  }
  static vector<std::string> &split(std::string const &target,
                                    char const &seperator = '\n') {
    string cur_code = "";
    vector<string> results;
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
};
