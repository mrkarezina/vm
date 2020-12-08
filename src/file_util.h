#ifndef FILE_UTIL_H
#define FILE_UTIL_H

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::shared_ptr<std::vector<std::string>> load_lines(std::string filename);
void write_lines(std::string filename,
                 std::shared_ptr<std::vector<std::string>> &lines);

#endif
