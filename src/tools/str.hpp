#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_STR_HPP
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_STR_HPP
#include <cstring>
#include <iostream>

#include "const_value.h"
using std::istream;
using std::ostream;
using std::string;
namespace sjtu {
#define STR_TEMPLATE template <int MaxLength>
#define STR_CLASS Str<MaxLength>
template <int MaxLength = kDefaultStrLength>
class Str;
STR_TEMPLATE
bool operator<(const STR_CLASS& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
bool operator>(const STR_CLASS& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
bool operator<=(const STR_CLASS& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
bool operator>=(const STR_CLASS& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
bool operator==(const STR_CLASS& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
bool operator!=(const STR_CLASS& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
std::istream& operator>>(std::istream& lhs, STR_CLASS& rhs);
STR_TEMPLATE
std::ostream& operator<<(std::ostream& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
STR_CLASS operator+(const STR_CLASS& lhs, const STR_CLASS& rhs);
STR_TEMPLATE
class Str {
 private:
  char ctx_[MaxLength + 1];  // +1 for '\0'

 public:
  Str();
  Str(const Str& other);
  Str(const string& other);
  Str(const char* other);
  int Length() const;
  Str& operator=(const Str& other);
  friend bool operator</**/<MaxLength>(const Str& lhs, const Str& rhs);
  friend bool operator> /**/<MaxLength>(const Str& lhs, const Str& rhs);
  friend bool operator<=<MaxLength>(const Str& lhs, const Str& rhs);
  friend bool operator>=<MaxLength>(const Str& lhs, const Str& rhs);
  friend bool operator==<MaxLength>(const Str& lhs, const Str& rhs);
  friend bool operator!=<MaxLength>(const Str& lhs, const Str& rhs);
  friend std::istream& operator>><MaxLength>(std::istream& lhs, Str& rhs);
  friend std::ostream& operator<<<MaxLength>(std::ostream& lhs, const Str& rhs);
  friend Str operator+<MaxLength>(const Str& lhs, const Str& rhs);
  string ToString() const;
  operator string() const;
  char& operator[](int index);
  const char& operator[](int index) const;
  ~Str();
};
STR_TEMPLATE
STR_CLASS::Str() { ctx_[0] = '\0'; }
STR_TEMPLATE
STR_CLASS::Str(const Str& other) { strcpy(ctx_, other.ctx_); }
STR_TEMPLATE
STR_CLASS::Str(const string& other) { strcpy(ctx_, other.c_str()); }
STR_TEMPLATE
STR_CLASS::Str(const char* other) { strcpy(ctx_, other); }
STR_TEMPLATE
int STR_CLASS::Length() const { return strlen(ctx_); }
STR_TEMPLATE
string STR_CLASS::ToString() const { return ctx_; }
STR_TEMPLATE
STR_CLASS::operator string() const { return ctx_; }
STR_TEMPLATE
STR_CLASS& STR_CLASS::operator=(const Str& other) {
  if (&other == this) return *this;
  strcpy(ctx_, other.ctx_);
  return *this;
}
STR_TEMPLATE
bool operator<(const STR_CLASS& lhs, const STR_CLASS& rhs) { return strcmp(lhs.ctx_, rhs.ctx_) < 0; }
STR_TEMPLATE
bool operator>(const STR_CLASS& lhs, const STR_CLASS& rhs) { return strcmp(lhs.ctx_, rhs.ctx_) > 0; }
STR_TEMPLATE
bool operator<=(const STR_CLASS& lhs, const STR_CLASS& rhs) { return strcmp(lhs.ctx_, rhs.ctx_) <= 0; }
STR_TEMPLATE
bool operator>=(const STR_CLASS& lhs, const STR_CLASS& rhs) { return strcmp(lhs.ctx_, rhs.ctx_) >= 0; }
STR_TEMPLATE
bool operator==(const STR_CLASS& lhs, const STR_CLASS& rhs) { return strcmp(lhs.ctx_, rhs.ctx_) == 0; }
STR_TEMPLATE
bool operator!=(const STR_CLASS& lhs, const STR_CLASS& rhs) { return strcmp(lhs.ctx_, rhs.ctx_) != 0; }
STR_TEMPLATE
std::istream& operator>>(std::istream& lhs, STR_CLASS& rhs) {
  string tmp;
  lhs >> tmp;
  rhs = tmp;
  return lhs;
}
STR_TEMPLATE
std::ostream& operator<<(std::ostream& lhs, const STR_CLASS& rhs) {
  lhs << rhs.ctx_;
  return lhs;
}
STR_TEMPLATE
STR_CLASS operator+(const STR_CLASS& lhs, const STR_CLASS& rhs) {
  STR_CLASS ret(lhs);
  strcpy(ret.ctx_ + ret.Length(), rhs.ctx_);
  return ret;
}
STR_TEMPLATE
char& STR_CLASS::operator[](int index) { return ctx_[index]; }
STR_TEMPLATE
const char& STR_CLASS::operator[](int index) const { return ctx_[index]; }
STR_TEMPLATE
STR_CLASS::~Str() = default;
using UserName = Str<kUserNameLength>;
using PassWord = Str<kPassWordLength>;
using Name = Str<kNameLength>;
using MailAddr = Str<kMailAddrLength>;
using TrainID = Str<kTrainIDLength>;
using Station = Str<kStationLength>;
STR_TEMPLATE
class StrHash {
 public:
  size_t operator()(const STR_CLASS& str) const {
    size_t ret = 0;
    for (int i = 0; i < str.Length(); ++i) ret = ret * 31 + str[i];
    return ret;
  }
};

using UserNameHash = StrHash<kUserNameLength>;
using PassWordHash = StrHash<kPassWordLength>;
using NameHash = StrHash<kNameLength>;
using MailAddrHash = StrHash<kMailAddrLength>;
using TrainIDHash = StrHash<kTrainIDLength>;
using StationHash = StrHash<kStationLength>;
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_STR_HPP