#ifndef TRAINTICKETSYSTEM_2022_SRC_TOOLS_SORT_HPP
#define TRAINTICKETSYSTEM_2022_SRC_TOOLS_SORT_HPP
#include <vector>

#include "vector.hpp"
namespace sjtu {
template <class T, class Cmp = std::less<T>>
void MergeSort_(std::vector<T>& vec, int st, int ed) {
  if (ed - st <= 1) return;
  int mid = (st + ed) / 2;
  MergeSort_(vec, st, mid);
  MergeSort_(vec, mid, ed);
  T* tmp = new T[ed - st];
  int i = st, j = mid, now = 0;
  for (; i < mid && j < ed;) tmp[now++] = Cmp()(vec[i], vec[j]) ? vec[i++] : vec[j++];
  for (; i < mid;) tmp[now++] = vec[i++];
  for (; j < ed;) tmp[now++] = vec[j++];
  for (i = st, now = 0; i < ed;) vec[i++] = tmp[now++];
  delete[] tmp;
  return;
}
template <class T, class Cmp = std::less<T>>
void Sort(std::vector<T>& vec) {
  MergeSort_(vec, 0, vec.size());
}
template <class T, class Cmp = std::less<T>>
void MergeSort_(sjtu::vector<T>& vec, int st, int ed) {
  if (ed - st <= 1) return;
  int mid = (st + ed) / 2;
  MergeSort_<T, Cmp>(vec, st, mid);
  MergeSort_<T, Cmp>(vec, mid, ed);
  T* tmp = new T[ed - st];
  int i = st, j = mid, now = 0;
  for (; i < mid && j < ed;) tmp[now++] = Cmp()(vec[i], vec[j]) ? vec[i++] : vec[j++];
  for (; i < mid;) tmp[now++] = vec[i++];
  for (; j < ed;) tmp[now++] = vec[j++];
  for (i = st, now = 0; i < ed;) vec[i++] = tmp[now++];
  delete[] tmp;
  return;
}
template <class T, class Cmp = std::less<T>>
void Sort(sjtu::vector<T>& vec) {
  MergeSort_<T, Cmp>(vec, 0, vec.size());
}
}  // namespace sjtu
#endif  // TRAINTICKETSYSTEM_2022_SRC_TOOLS_SORT_HPP