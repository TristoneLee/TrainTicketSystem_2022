#ifndef BOOKSTORE_SRC_FILESYSTEM_FILEPTR_H
#define BOOKSTORE_SRC_FILESYSTEM_FILEPTR_H
template <class ValueType>
class FilePtr;
template <class ValueType>
FilePtr<ValueType> operator+(const FilePtr<ValueType>&, const int&);
template <class ValueType>
class FilePtr {
 private:
  long pos;

 public:
  FilePtr();
  FilePtr(long);
  FilePtr(const FilePtr<ValueType>&);
	void Clear();
  long Get_Pos() const;
  operator bool()const;
	bool operator!()const;	
  FilePtr<ValueType>& operator++();
  FilePtr<ValueType>& operator--();
  FilePtr<ValueType> operator++(int);

  friend FilePtr<ValueType> operator+
      <ValueType>(const FilePtr<ValueType>&, const int&);
};
#include "FilePtr.inl"
#endif  // BOOKSTORE_SRC_FILESYSTEM_FILEPTR_H