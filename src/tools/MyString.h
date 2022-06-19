#ifndef BOOKSTORE_SRC_FILESYSTEM_MYSTRING_H
#define BOOKSTORE_SRC_FILESYSTEM_MYSTRING_H
#include <cstring>
#include <string>
#include "../FileSystem/DefaultConst.hpp"
extern const int string_size;
using std::string;
// class MyString;
class MyString {
 private:
  char str[string_size];

 public:
  MyString();
  MyString(const string& other);
  MyString(const char* other);
  int Length()const;
	MyString& operator=(const MyString& other);
	friend bool operator<(const MyString& lhs,const MyString& rhs);
	friend bool operator>(const MyString& lhs,const MyString& rhs);
	friend bool operator<=(const MyString& lhs,const MyString& rhs);
	friend bool operator>=(const MyString& lhs,const MyString& rhs);
	friend bool operator==(const MyString& lhs,const MyString& rhs);
	friend bool operator!=(const MyString& lhs,const MyString& rhs);
	friend std::istream& operator>>(std::istream& lhs,MyString& rhs);
	friend std::ostream& operator<<(std::ostream& lhs,const MyString& rhs);
	string ToString()const;
	operator string();
	char& operator[](int index);
	char operator[](int index)const;
};
#endif  // BOOKSTORE_SRC_FILESYSTEM_MYSTRING_H