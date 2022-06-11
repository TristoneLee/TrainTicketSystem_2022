#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_CMD_SCANNER_CPP
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_CMD_SCANNER_CPP
#include "cmd_scanner.h"

#include <iostream>

#include "map.hpp"
using std::istream;
using std::istringstream;
using std::string;
namespace sjtu {
void Command::ScanCommand() {
  if (buffer_.empty()) return;
  auto iter = buffer_.begin(), ed_iter = buffer_.end();
  char tmp_c;
  auto clear_space = [&]() -> void {
    while (iter != ed_iter && *iter == ' ') ++iter;
  };

  auto get_token = [&](string& obj) -> void {
    while (iter != ed_iter && *iter != ' ') {
      obj.push_back(*iter);
      ++iter;
    }
  };
  clear_space();
  get_token(op);
  clear_space();
  while (iter != ed_iter) {
    // now iter points to '-'
    ++iter;
    // now iter points to the key
    char key = *iter;
    ++iter;
    clear_space();
    string tmp_value;
    get_token(tmp_value);
    clear_space();
    args[key] = tmp_value;
  }
}
Command::Command(istream& input_stream) {
  getline(input_stream, buffer_);
  ScanCommand();
}
Command::Command(const string& str) : buffer_(str) { ScanCommand(); }
Command::~Command() = default;
};      // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_CMD_SCANNER_CPP