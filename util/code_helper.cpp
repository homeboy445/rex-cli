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

vector<std::pair<int, std::string>>
code_helper::find(std::string const &target) {
  cout << " << " << target << " >> \n";
  vector<std::pair<int, std::string>> results;
  for (int i = 0; i < this->code.size(); ++i) {
    cout << "line number " << i + 1 << " <<\n";
    if (code[i] == "\n" || code[i] == "" || code[i] == " ")
      continue;
    auto index = code[i].find(target);
    if (index != string::npos) { // TODO: Implement a better algorithm for
                                 // finding substring!
      results.push_back({i, code[i]});
    }
  }
  return results;
}
