#ifndef BOOKSTORE_SRC_FILESYSTEM_FILEMEMPOOL_CPP
#define BOOKSTORE_SRC_FILESYSTEM_FILEMEMPOOL_CPP
#include <fstream>
#include <string>

#include "MyString.h"
using std::fstream;
using std::string;
#include "FileMemPool.h"
bool fileop::File_Exist(MyString file_name) {
  fstream temp_file(file_name.ToString(),
                    fstream::in | fstream::out | fstream::binary);
  if (!temp_file) return false;
  temp_file.close();
  return true;
}
void fileop::Create_File(MyString file_name) {
  fstream temp_file(file_name.ToString(), fstream::out | fstream::binary);
  temp_file.close();
  return;
}
FileMemPool::FileMemPool(string _file_name)
    : file_name(_file_name.append(".dat")) {
  if (!fileop::File_Exist(file_name)) fileop::Create_File(file_name);
}
FileMemPool::FileMemPool(const char* _file_name)
    : FileMemPool(string(_file_name)) {}
bool FileMemPool::Empty() const {
  fstream temp_file(file_name.ToString(),
                    fstream::in | fstream::out | fstream::binary);
  temp_file.seekp(0, std::ios::end);
  bool tmp_ans = temp_file.tellp();
  temp_file.close();
  return !tmp_ans;
}
FileMemPool::~FileMemPool() = default;

#endif  // BOOKSTORE_SRC_FILESYSTEM_FILEMEMPOOL_CPP
