#include "../include/code_helper.h"

#include <fstream>

using namespace std;

code_helper::code_helper(string const &path) {
  this->path = path;
  fstream file;
  file.open(path, ios::in);
  string data, tmp;
  while (getline(file, tmp))
    data += tmp + "\n";
  file.close();
  this->code = utility::split(data);
}

vector<std::pair<int, std::string>> code_helper::find(std::string const &target,
                                                      int const &lines) {
  vector<std::pair<int, std::string>> results;
  for (int i = 0; i < this->code.size(); ++i) {
    if (code[i] == "\n" || code[i] == "" || code[i] == " ")
      continue;
    if (code[i].find(target) !=
        string::npos) { // TODO: Implement a better algorithm for
                        // finding substring!
      if (lines > 0) {
        string s = code[i] + '\n';
        int j = i - 1, lines_ = lines;
        while (j >= 0 && lines_ > 0) {
          s = code[j--] + "\n" + s, lines_--;
        }
        j = i + 1, lines_ = lines;
        while (j < code.size() && lines_ > 0) {
          s += code[j++] + "\n", lines_--;
        }
        results.push_back({i, s});
      } else {
        results.push_back({i, code[i]});
      }
    }
  }
  return results;
}
