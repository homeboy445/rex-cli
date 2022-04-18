#include "../include/code_helper.h"

#include <fstream>
#include <unordered_map>

using namespace std;

code_helper::code_helper(string const &path) {
  try {
    this->path = path;
    fstream file;
    file.open(path, ios::in);
    string data, tmp;
    while (getline(file, tmp))
      data += tmp + "\n";
    file.close();
    this->code = utility::split(data);
  } catch (std::exception const &e) {
    throw e.what();
  }
}

void code_helper::save() {
  fstream file;
  file.open(this->path, ios::out);
  for (auto const &line : this->code) {
    file << line + '\n';
  }
  file.close();
}

/**
 * @brief Helper function to find keywords within a codebase(or a paragraph and
 * similar). Specify the target and the lines to be displayed above and below
 * the found target code to use the method(by default 0).
 *
 * @param target
 * @param lines
 * @return vector<std::pair<int, std::string>>
 */
vector<std::pair<int, std::string>> code_helper::find(std::string const &target,
                                                      int const &lines) {
  vector<std::pair<int, std::string>> results;
  auto find_tar_multiline = [&](string const &source,
                                string const &target_code) -> bool {
    int found = 0;
    vector<string> candidates = utility::split(target_code);
    for (auto const &candidate : utility::split(target_code)) {
      if (source.find(candidate) != std::string::npos) {
        found++;
      }
    }
    return found ==
           int(candidates.size()); // Ensure that everything has been found...!
  };
  for (int i = 0; i < this->code.size(); ++i) {
    if (code[i] == "\n" || code[i] == "" || code[i] == " ") {
      continue;
    }
    bool multiline = false;
    if (target.find('\n') != string::npos) {
      multiline = true;
    }
    if (code[i].find(target) != string::npos ||
        (multiline &&
         find_tar_multiline(code[i],
                            target))) { // TODO: Implement a better algorithm
                                        // for finding substring!
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

vector<pair<int, pair<string, string>>>
code_helper::replace(std::string const &target, std::string const &replacer) {
  vector<pair<int, pair<string, string>>> results;
  for (int i = 0; i < code.size(); i++) {
    size_t index = code[i].find(target);
    string tmp = code[i];
    bool found = false;
    while (index != string::npos) {
      found = true; // TODO: Devise a better approach!
      string s;
      for (int j = 0, flag = 1; j < code[i].size(); j++) {
        if (j == index) {
          s += replacer, j += target.length() - 1;
          continue;
        }
        s.push_back(code[i][j]);
      }
      code[i] = s;
      index = code[i].find(target);
    }
    if (found) {
      results.push_back({i, {tmp, code[i]}});
    }
  }
  this->save();
  return results;
}

void code_helper::revert() { this->save(); }
