#include <iostream>

#include "cmd_scanner.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
int main() {
  #ifndef ONLINE_JUDGE
  freopen("stdin.txt","r",stdin);
  freopen("stdout.txt","w",stdout);
  #endif
  for (int i = 1; i <= 5; i++) {
    sjtu::Command cmd(cin);
    cout << "----cmd:" << i << "----" << endl;
    cout << "op : " << cmd.op << endl;
    cout << "arg:" << endl;
    for (auto it = cmd.args.begin(); it != cmd.args.end(); ++it) {
      cout << "(" << it->first << ") : (" << it->second << ")" << endl;
    }
    cout << "----end:" << i << "----" << endl;
  }
}