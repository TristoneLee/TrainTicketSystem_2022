#include <iostream>

#include "database.h"
#include "sort.hpp"
#include "str.hpp"
#include "user.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
int main() {
#ifndef ONLINE_JUDGE
  // freopen("stdin.txt", "r", stdin);
  // freopen("stdout.txt", "w", stdout);
#endif
  sjtu::bpTree<sjtu::UserName, sjtu::User> a("a");
  a.insert("I_am_the_admin", sjtu::User());
  a.insert("Croissant", sjtu::User());
  a.insert("Mayer", sjtu::User());
  auto ret = a.query("Mayer");
  cout << ret.size() << endl;
  return 0;
}