#ifndef TRAINTICKETSYSTEM_2022_SRC_DATA_USER_H
#define TRAINTICKETSYSTEM_2022_SRC_DATA_USER_H
#include "str.hpp"
namespace sjtu {
class User {
 public:
  UserName username;
  Name name;
  PassWord password;
  MailAddr mail_addr;
  int privilege;
  User(const string& _user_name, const string& _name, const string& _password,
       const string& _mail_addr, int _privilege);
  ~User();
};
};      // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_DATA_USER_H