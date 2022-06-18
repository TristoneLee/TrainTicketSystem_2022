#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_SPLIT_CPP
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_SPLIT_CPP
#include <iostream>

using std::string;
#include "asserts.h"
#include "split.h"
#include "vector.hpp"

namespace sjtu {
int ToInt(const string& str) {
  int ret = 0;
  for (const char& c : str) {
    Assert(c >= '0' && c <= '9', "ToInt fail: invalid char");
    ret = ret * 10 + c - '0';
  }
  return ret;
}

vector<string> SplitIntoStrings(const string& str) {
  // split a string into a vector of string by "|"
  vector<string> ret;
  if (str == "_") return ret;
  string tmp;
  for (const char& c : str) {
    if (c == '|') {
      ret.push_back(tmp);
      tmp.clear();
    } else
      tmp.push_back(c);
  }
  if (!tmp.empty()) ret.push_back(tmp);
  return ret;
}

vector<int> SplitIntoInts(const string& str) {
  // split a string into a vector of int by "|"
  vector<string> tmp = SplitIntoStrings(str);
  vector<int> ret;
  for (const string& s : tmp) ret.push_back(ToInt(s));
  return ret;
}

vector<Date> SplitIntoDates(const string& str) {
  // split a string into a vector of int by "|"
  vector<string> tmp = SplitIntoStrings(str);
  vector<Date> ret;
  for (const string& s : tmp) ret.push_back(s);
  return ret;
}
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_SPLIT_CPP