#ifndef BOOKSTORE_SRC_FILESYSTEM_COMP_INL
#define BOOKSTORE_SRC_FILESYSTEM_COMP_INL
#include <utility>

#include "Comp.h"
using std::pair;
template <class ValueType>
int Comp::SingleComp<ValueType>::operator()(const ValueType& lhs,
                                            const ValueType& rhs) {
  if (lhs < rhs) return -1;
  if (rhs < lhs) return 1;
  return 0;
}
template <class KeyValueFirst, class KeyValueSecond>
int Comp::PairCompFirst<KeyValueFirst, KeyValueSecond>::operator()(
    const pair<KeyValueFirst, KeyValueSecond>& lhs,
    const pair<KeyValueFirst, KeyValueSecond>& rhs) {
  return Comp::SingleComp<KeyValueFirst>()(lhs.first, rhs.first);
}
template <class Compare, class ValueType>
bool Comp::CompToLess<Compare, ValueType>::operator()(const ValueType& lhs,
                                                      const ValueType& rhs) {
  return Compare()(lhs, rhs) < 0;
}
template <class Compare, class ValueType>
bool Comp::LessToComp<Compare, ValueType>::operator()(const ValueType& lhs,
                                                      const ValueType& rhs) {
  if (Compare()(lhs, rhs)) return -1;
  if (Compare()(rhs, lhs)) return 1;
  return 0;
}

template <class ValueType>
ValueType Comp::GetSelf<ValueType>::operator()(const ValueType& obj) {
  return obj;
}

template <class ValueType>
int Comp::BailanHash<ValueType>::operator()(const ValueType& obj) {
  return 0;
}
#endif  // BOOKSTORE_SRC_FILESYSTEM_COMP_INL