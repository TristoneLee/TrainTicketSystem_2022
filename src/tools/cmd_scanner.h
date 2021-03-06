#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_CMD_SCANNER_H
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_CMD_SCANNER_H
#include <iostream>

#include "map.hpp"
using std::istream;
using std::string;
namespace sjtu {
class Command {
 private:
  string buffer_;
  void ScanCommand();

 public:
  int timestamp;
  string op;
  map<char, string> args;
  Command(istream& input_stream);
  Command(const string& str);
  ~Command();
};
};      // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_CMD_SCANNER_H