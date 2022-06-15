#ifndef BOOKSTORE_SRC_FILESYSTEM_FILEPTR_INL
#define BOOKSTORE_SRC_FILESYSTEM_FILEPTR_INL
#include "FilePtr.h"
template <class ValueType>
FilePtr<ValueType>::FilePtr() : pos(-1){};

template <class ValueType>
FilePtr<ValueType>::FilePtr(long _pos) : pos(_pos){};

template <class ValueType>
FilePtr<ValueType>::FilePtr(const FilePtr<ValueType>& other) : pos(other.pos){};

template <class ValueType>
void FilePtr<ValueType>::Clear()
{
	pos = -1;
}

template <class ValueType>
long FilePtr<ValueType>::Get_Pos() const {
  return pos;
}
template <class ValueType>
FilePtr<ValueType>::operator bool() const {
  return pos != -1;
}
template <class ValueType>
bool FilePtr<ValueType>::operator!() const {
  return pos == -1;
}
template <class ValueType>
FilePtr<ValueType>& FilePtr<ValueType>::operator++() {
  pos += sizeof(ValueType);
  return *this;
}
template <class ValueType>
FilePtr<ValueType>& FilePtr<ValueType>::operator--() {
  pos -= sizeof(ValueType);
  return *this;
}
template <class ValueType>
FilePtr<ValueType> FilePtr<ValueType>::operator++(int) {
  FilePtr<ValueType> ans(*this);
  pos += sizeof(ValueType);
  return ans;
}
template <class ValueType>
FilePtr<ValueType> operator+(const FilePtr<ValueType>& lhs, const int& rhs) {
  return FilePtr<ValueType>(lhs.pos + rhs * sizeof(ValueType));
}
#endif  // BOOKSTORE_SRC_FILESYSTEM_FILEPTR_INL