#ifndef BOOKSTORE_SRC_FILESYSTEM_FILEMEMPOOL_H
#define BOOKSTORE_SRC_FILESYSTEM_FILEMEMPOOL_H
#include <fstream>
#include <string>
#include "MyString.h"
#include "FilePtr.h"
using std::fstream;
using std::string;
namespace fileop {
bool File_Exist(MyString);
void Create_File(MyString file_name);
}  // namespace fileop
class FileMemPool {
 private:
  MyString file_name;

 public:
  FileMemPool(string);
  FileMemPool(const char*);
  template <class ValueType>
  FilePtr<ValueType> fnew(const ValueType&, const int& num = 1);
  template <class ValueType>
  ValueType fread(const FilePtr<ValueType>&)const;
  template <class ValueType>
  void fread(const FilePtr<ValueType>&, ValueType& ans)const;
  template <class ValueType>
  void modify(const FilePtr<ValueType>& ptr, const ValueType& obj);
	bool Empty()const;
  ~FileMemPool();
};
#include "FileMemPool.inl"
#endif  // BOOKSTORE_SRC_FILESYSTEM_FILEMEMPOOL_H