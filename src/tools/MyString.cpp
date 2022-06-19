#ifndef BOOKSTORE_SRC_FILESYSTEM_MYSTRING_CPP
#define BOOKSTORE_SRC_FILESYSTEM_MYSTRING_CPP
#include "MyString.h"
#include <cstring>
#include <string>
#include <iostream>
using std::string;
MyString::MyString() { str[0] = '\0'; }
MyString::MyString(const string& other) { strcpy(str, other.c_str()); }
MyString::MyString(const char* other){strcpy(str, other);}
int MyString::Length() const { return strlen(str); }
string MyString::ToString() const { return str; }
MyString& MyString::operator=(const MyString& other) {
  if (&other == this) return *this;
  strcpy(str, other.str);
  return *this;
}
bool operator<(const MyString& lhs, const MyString& rhs) {
  return strcmp(lhs.str, rhs.str) < 0;
}
bool operator>(const MyString& lhs, const MyString& rhs) {
  return strcmp(lhs.str, rhs.str) > 0;
}
bool operator<=(const MyString& lhs, const MyString& rhs) {
  return strcmp(lhs.str, rhs.str) <= 0;
}
bool operator>=(const MyString& lhs, const MyString& rhs) {
  return strcmp(lhs.str, rhs.str) >= 0;
}
bool operator==(const MyString& lhs, const MyString& rhs) {
  return strcmp(lhs.str, rhs.str) == 0;
}
bool operator!=(const MyString& lhs, const MyString& rhs) {
  return strcmp(lhs.str, rhs.str) != 0;
}
std::istream& operator>>(std::istream& lhs,MyString& rhs) {
	string tmp;
	lhs>>tmp;
	rhs = tmp;
	return lhs;
}
std::ostream& operator<<(std::ostream& lhs,const MyString& rhs) {
	lhs<<rhs.ToString();
	return lhs;
}
MyString operator+(const MyString& lhs, const MyString& rhs) {
  return lhs.ToString() + rhs.ToString();
}
char& MyString::operator[](int index) { return str[index]; }
char MyString::operator[](int index) const { return str[index]; }
#endif  // BOOKSTORE_SRC_FILESYSTEM_MYSTRING_CPP