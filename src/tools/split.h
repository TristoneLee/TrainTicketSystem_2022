#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_SPLIT_H
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_SPLIT_H

#include <iostream>

using std::string;
#include "asserts.h"
#include "times.h"
#include "vector.hpp"
namespace sjtu {
int ToInt(const string& str);
vector<string> SplitIntoStrings(const string& str);
vector<int> SplitIntoInts(const string& str);
vector<Date> SplitIntoDates(const string& str);
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_SPLIT_H