#ifndef TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_USER_SYSTEM_CPP
#define TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_USER_SYSTEM_CPP
#include "user_system.h"

#include <iostream>

#include "asserts.h"
using std::string;
namespace sjtu {
UserSystem::UserSystem() : users_("UserSystem_users_") {}
void UserSystem::AddUser(const string& username, const string& name, const string& password, const string& mail_addr,
                         int privilege) {
  Assert(users_.query(username).empty());
  User user(username, name, password, mail_addr, privilege);
  users_.insert(username, user);
  return;
}
void UserSystem::Login(const string& username, const string& password) {
  Assert(string(QueryProfile(username).password) == password, "login fail: wrong password");
  Assert(!IsOnline(username), "login fail: already online");
  online_status_[username] = true;
  return;
}
void UserSystem::Logout(const string& username) {
  Assert(IsOnline(username), "logout fail: not online");
  online_status_.erase(online_status_.find(username));
  return;
}
User UserSystem::QueryProfile(const string& username) {
  auto find_ret = users_.query(username);
  Assert(!find_ret.empty(), "query profile fail: not found");
  return find_ret[0];
}
void UserSystem::ModifyProfile(const User& ori_val, const User& new_val) {
  users_.erase(ori_val.username, ori_val);
  users_.insert(new_val.username, new_val);
}
bool UserSystem::IsOnline(const string& username) { return online_status_.count(username); }
UserSystem::~UserSystem() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_SYSTEMS_USER_SYSTEM_CPP