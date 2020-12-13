#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <vector>

template <typename T>
bool vec_contains(std::vector<T> v, T e) {
  return std::find(v.begin(), v.end(), e) != v.end();
}

#endif
