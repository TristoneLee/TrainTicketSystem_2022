#pragma once
#ifndef BOOKSTORE_SRC_FILESYSTEM_BPTREE_NODE_INL
#define BOOKSTORE_SRC_FILESYSTEM_BPTREE_NODE_INL
#include "BPTree.h"
#pragma region Done
template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare, KeyCompare,
        IndexLess>::Node::Node()
    : size(0), is_leaf(true) {}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare, KeyCompare,
        IndexLess>::Node::Node(const FilePtr<Node> &_self)
    : Node() {
  self = _self;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Node::BecomeParent(Node &lchild,
                                                        Node &rchild) {
  if (size) throw("A non-empty Node became the parent of other Nodes");
	is_leaf = false;
  size = 2;
  children[0] = lchild.self;
  children[1] = rchild.self;
  lchild.parent = self;
  rchild.parent = self;
  index[0] = lchild.index[0];
  index[1] = rchild.index[0];
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Node::InsertCtx(const IndexType &tmp_index,
                                                     const FilePtr<ValueType>
                                                         &value) {
  int pos = LowerBound(tmp_index);
  InsertCtx(tmp_index, value, pos);
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Node::InsertCtx(const IndexType &tmp_index,
                                                     const FilePtr<ValueType>
                                                         &value,
                                                     int pos) {
  index[size] = tmp_index;
  ctx[size] = value;
  Move(pos, size);
  size++;
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
int bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
            KeyCompare, IndexLess>::Node::InsertNode(Node &obj) {
  int pos = LowerBound(obj.index[0]);
  InsertNode(obj, pos);
  return pos;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Node::InsertNode(Node &obj, int pos) {
  // if (pos == size) {
  //   if (nxt) {
  //     Node nxt_node(nxt);
  //   }
  // } else {
  //   obj.nxt = children[pos];
  // }
  if (pos == 0) {
  } else {
    obj.lst = children[pos - 1];
  }
  index[size] = obj.index[0];
  children[size] = obj.self;
  Move(pos, size);
  size++;
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Node::Move(int lpos, int rpos) {
  if (lpos > rpos) std::swap(lpos, rpos);
  if (rpos > size) throw("Invaild Move");
  while (lpos < rpos) {
    std::swap(index[rpos], index[rpos - 1]);
    std::swap(children[rpos], children[rpos - 1]);
    std::swap(ctx[rpos], ctx[rpos - 1]);
    rpos--;
  }
  return;
}

/* template <class KeyType, class ValueType, class ValueIndexType, class
HashFunc, class IndexCompare, class KeyCompare, class IndexLess> int
bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare, KeyCompare,
IndexLess>::Node::LowerBound(const KeyType &key, const ValueType &value) {
  IndexType tmp_index = std::make_pair(key, HashFunc()(value));
  return std::lower_bound(index, index + size, tmp_index, IndexLess()) - index;
} */

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
int bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
            KeyCompare, IndexLess>::Node::LowerBound(const IndexType
                                                         &tmp_index) {
  return std::lower_bound(index, index + size, tmp_index, IndexLess()) - index;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
int bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
            KeyCompare, IndexLess>::Node::LowerBound(const KeyType &key) {
  IndexType tmp_index;
  tmp_index.first = key;
  return std::lower_bound(index, index + size, tmp_index, IndexCompare()) -
         index;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Node::HalfSpilt(Node &ans) {
  ans.size = size >> 1;
  size = size - (size >> 1);
  ans.is_leaf = is_leaf;
  for (int i = 0; i < ans.size; i++) {
    ans.index[i] = index[size + i];
    ans.children[i] = children[size + i];
    ans.ctx[i] = ctx[size + i];
  }
  ans.lst = self;
  ans.nxt = nxt;
  nxt = ans.self;
  return;
}
#pragma endregion
#endif  // BOOKSTORE_SRC_FILESYSTEM_BPTREE_NODE_INL