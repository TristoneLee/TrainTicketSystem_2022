#ifndef BOOKSTORE_SRC_FILESYSTEM_COMP_H
#define BOOKSTORE_SRC_FILESYSTEM_COMP_H
#include <utility>
using std::pair;
namespace Comp {
template <class ValueType>
class SingleComp {
 public:
  int operator()(const ValueType& lhs, const ValueType& rhs);
};
template <class KeyValueFirst, class KeyValueSecond>
class PairCompFirst {
 public:
  int operator()(const pair<KeyValueFirst, KeyValueSecond>& lhs,
                 const pair<KeyValueFirst, KeyValueSecond>& rhs);
};
template <class Compare, class ValueType>
class CompToLess {
 public:
  bool operator()(const ValueType& lhs, const ValueType& rhs);
};
template <class Compare, class ValueType>
class LessToComp {
 public:
  bool operator()(const ValueType& lhs, const ValueType& rhs);
};
template <class ValueType>

class GetSelf {
 public:
  ValueType operator()(const ValueType& obj);
};

template <class ValueType>
class BailanHash {
 public:
  int operator()(const ValueType& obj);
};
}  // namespace Comp
#include "Comp.inl"
#endif  // BOOKSTORE_SRC_FILESYSTEM_COMP_H