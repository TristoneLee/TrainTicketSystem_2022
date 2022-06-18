#include <iostream>

#include "sort.hpp"
#include "str.hpp"
using std::cin;
using std::cout;
using std::endl;
using std::string;
int main() {
#ifndef ONLINE_JUDGE
  freopen("stdin.txt", "r", stdin);
  freopen("stdout.txt", "w", stdout);
#endif
  sjtu::vector<sjtu::TrainID> vec;
  vec.push_back("abc");
  vec.push_back("def");
  vec.push_back("aa");
  vec.push_back("bb");
  for (auto i : vec) cout << i << endl;
  sjtu::Sort(vec);
  for (auto i : vec) cout << i << endl;
  return 0;
}