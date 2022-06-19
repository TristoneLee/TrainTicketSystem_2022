#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_USER_SYSTEM_H
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_USER_SYSTEM_H
#include <iostream>
using std::string;
#include "database.h"
#include "linked_hashmap.hpp"
#include "user.h"
namespace sjtu {
class UserSystem {
 private:
  linked_hashmap<string, bool> online_status_;
  bpTree<UserName, User> users_;

 public:
  UserSystem();
  void AddUser(const string& username, const string& name, const string& password, const string& mail_addr,
               int privilege);
  void Login(const string& username, const string& password);
  void Logout(const string& username);
  User QueryProfile(const string& username);
  void ModifyProfile(const User& ori_val, const User& val);
  bool IsOnline(const string& username);
  ~UserSystem();
};
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_USER_SYSTEM_H