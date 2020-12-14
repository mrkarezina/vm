#include "file_util.h"

using namespace std;

shared_ptr<vector<string>> load_lines(string filename) {
  shared_ptr<vector<string>> lines = make_shared<vector<string>>();
  std::ifstream file;
  file.open(filename);
  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      lines->push_back(line);
    }
    file.close();
  } else {
    cerr << "Failed to open file: " << filename << endl;
    exit(1);
  }
  return lines;
}

void write_lines(string filename, shared_ptr<vector<string>> &lines) {
  ofstream file(filename.size() > 0 ? filename : "default_vm.txt");
  for (auto l : *lines) {
    file << l << endl;
  }
  file.close();
}
