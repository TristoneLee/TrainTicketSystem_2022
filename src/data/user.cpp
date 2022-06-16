#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_USER_CPP
#define TRAINTICKETSYSTEM_2022_SRC_DATA_USER_CPP
#include "user.h"
namespace sjtu {
User::User() = default;
User::User(const string& _user_name, const string& _name, const string& _password, const string& _mail_addr,
           int _privilege)
    : username(_user_name), name(_name), password(_password), mail_addr(_mail_addr), privilege(_privilege) {}
User& User::operator=(const User& other) = default;
ostream& operator<<(ostream& out, const User& user) {
  out << user.username << ' ' << user.name << ' ' << user.mail_addr << ' ' << user.privilege;
  return out;
}
User::~User() = default;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_USER_CPP