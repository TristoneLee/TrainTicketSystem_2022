#ifndef BOOKSTORE_SRC_BPTREE_INL
#define BOOKSTORE_SRC_BPTREE_INL
#include <cmath>
#include <iostream>

#include "BPTree.h"
#include "FileMemPool.h"
#pragma region done
template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Load() {
  node_file.fread(FilePtr<bpTree<KeyType, ValueType, ValueIndexType, HashFunc,
                                  IndexCompare, KeyCompare, IndexLess>>(0),
                  *this);
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Init() {
  node_file.fnew(*this);
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Save() {
  node_file.modify(FilePtr<bpTree<KeyType, ValueType, ValueIndexType, HashFunc,
                                   IndexCompare, KeyCompare, IndexLess>>(0),
                   *this);
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::Print() {
  DFSPrint(root);
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::DFSPrint(FilePtr<Node> now) {
  static string tmptab = "";
  Node now_node(now);
  LoadNode(now_node);
  if (now_node.is_leaf) {
    std::cout << tmptab << "Node : " << now.Get_Pos()
              << " size : " << now_node.size
              << " is leaf , value :" << std::endl;
    for (int i = 0; i < now_node.size; i++) {
      if (now_node.ctx[i])
        std::cout << tmptab << "index[" << i << "] = ("
                  << now_node.index[i].first << ", " << now_node.index[i].second
                  << ") , value = " << GetValue(now_node.ctx[i]) << std::endl;
      else
        std::cout << tmptab << "index[" << i << "] = ("
                  << now_node.index[i].first << ", " << now_node.index[i].second
                  << ") , value = "
                  << "NONE" << std::endl;
    }
    return;
  }
  std::cout << tmptab << "Node : " << now.Get_Pos()
            << " size : " << now_node.size << " is branch :" << std::endl;
  std::cout << tmptab << "lst : " << now_node.lst.Get_Pos()
            << " nxt : " << now_node.nxt.Get_Pos() << std::endl;
  std::cout << tmptab << "index:" << std::endl;
  for (int i = 0; i < now_node.size; i++)
    std::cout << tmptab << "index[" << i << "] = (" << now_node.index[i].first
              << ", " << now_node.index[i].second << ")" << std::endl;
  std::cout << tmptab << "childrens : " << std::endl;
  tmptab += '\t';
  for (int i = 0; i < now_node.size; i++) DFSPrint(now_node.children[i]);
  tmptab.pop_back();
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
typename bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
                 KeyCompare, IndexLess>::Node
bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare, KeyCompare,
        IndexLess>::CreatNode() {
  Node ans;
  ans.self = node_file.fnew(ans);
  UpdateNode(ans);
  return ans;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::StoreNode(Node& obj) {
  if (obj.self) throw("Store a existing Node");
  obj.self = node_file.fnew(obj);
  UpdateNode(obj);
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::LoadNode(Node& obj) const {
  if (!obj.self) throw("Load a non-existing Node");
  obj = node_file.fread(obj.self);
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::UpdateNode(const Node& obj) {
  if (!obj.self) throw(obj.self.Get_Pos());
  if (!obj.self) throw("Update a non-existing Node");
  node_file.modify(obj.self, obj);
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
ValueType
bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare, KeyCompare,
        IndexLess>::GetValue(const FilePtr<ValueType>& ptr) {
  return ctx_file.fread(ptr);
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare, KeyCompare,
        IndexLess>::bpTree(const string& _file_name)
    : node_file(_file_name + "_node"), ctx_file(_file_name + "_ctx") {
  if (node_file.Empty())
    Init();
  else
    Load();
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::SplitRoot() {
  Node root_node(root);
  LoadNode(root_node);
  Node back_half = CreatNode();
  root_node.HalfSpilt(back_half);
  Node new_root_node = CreatNode();
  new_root_node.BecomeParent(root_node, back_half);
  UpdateNode(back_half);
  UpdateNode(root_node);
  UpdateNode(new_root_node);
  LoadNode(root_node);
  root = new_root_node.self;
  Save();
  return;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::insert(const KeyType& key,
                                            const ValueType& value) {
  auto ctx_ptr = ctx_file.fnew(value);
  if (!root) {
    Node root_node = CreatNode();
    root = root_node.self;
    root_node.is_leaf = true;
    root_node.InsertCtx(std::make_pair(key, HashFunc()(value)), ctx_ptr);
    UpdateNode(root_node);
    Save();
    return;
  }
  Node root_node(root);
  LoadNode(root_node);
  DFSInsert(std::make_pair(key, HashFunc()(value)), ctx_ptr, root_node);
  UpdateNode(root_node);
  if (root_node.size > max_node_size) SplitRoot();
  return;
}

// #pragma endregion
template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::DFSInsert(const IndexType& tmp_index,
                                               const FilePtr<ValueType>& value,
                                               Node& now_node) {
  if (now_node.is_leaf) {
    now_node.InsertCtx(tmp_index, value);
    UpdateNode(now_node);
    return;
  }
  int pos = std::max(0, now_node.LowerBound(tmp_index) - 1);
  Node nxt_node(now_node.children[pos]);
  LoadNode(nxt_node);
  DFSInsert(tmp_index, value, nxt_node);
  now_node.index[pos] = nxt_node.index[0];
  if ((nxt_node.is_leaf && nxt_node.size >= max_node_size) ||
      nxt_node.size > max_node_size) {
    Node back_half = CreatNode();
    nxt_node.HalfSpilt(back_half);
    now_node.InsertNode(back_half, pos + 1);
    UpdateNode(nxt_node);
    UpdateNode(back_half);
  } /*  else if (nxt_node.size > max_node_size) {

 } */
  UpdateNode(now_node);
}
template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
vector<ValueType>
bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare, KeyCompare,
        IndexLess>::query(const KeyType& key) {
  vector<ValueType> ans;
  if (!root) return ans;
  Node root_node(root);
  LoadNode(root_node);
  DFSFind(key, root_node, ans);
  return ans;
}

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::DFSFind(const KeyType& key, Node& now_node,
                                             vector<ValueType>& ans) {
  if (now_node.is_leaf) {
    int pos = now_node.LowerBound(key);
    while (true) {
      for (; pos < now_node.size; pos++) {
        if (KeyCompare()(now_node.index[pos].first, key)) return;
        if (now_node.ctx[pos]) ans.push_back(GetValue(now_node.ctx[pos]));
      }
      if (!now_node.nxt) return;
      now_node.self = now_node.nxt;
      LoadNode(now_node);
      pos = 0;
    }
    throw("Unkown Error");
    return;
  }
  int pos = std::max(now_node.LowerBound(key) - 1, 0);
  Node nxt_node(now_node.children[pos]);
  LoadNode(nxt_node);
  DFSFind(key, nxt_node, ans);
  return;
}
template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::erase(const KeyType& key,
                                           const ValueType& value) {
  IndexType tmp_index = std::make_pair(key, HashFunc()(value));
  if (!root) return;
  Node root_node(root);
  LoadNode(root_node);
  DFSErase(tmp_index, root_node);
}
template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
void bpTree<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
             KeyCompare, IndexLess>::DFSErase(const IndexType& tmp_index,
                                              Node& now_node) {
  if (now_node.is_leaf) {
    int pos = now_node.LowerBound(tmp_index);
    typedef Comp::LessToComp<IndexLess, IndexType> IndexRealComp;
    while (true) {
      for (; pos < now_node.size; pos++) {
        if (IndexRealComp()(now_node.index[pos], tmp_index)) return;
        now_node.ctx[pos].Clear();
        UpdateNode(now_node);
      }
      if (!now_node.nxt) return;
      now_node.self = now_node.nxt;
      LoadNode(now_node);
      pos = 0;
    }
    throw("Unkown Error");
    return;
  }
  int pos = std::max(now_node.LowerBound(tmp_index) - 1, 0);
  Node nxt_node(now_node.children[pos]);
  LoadNode(nxt_node);
  DFSErase(tmp_index, nxt_node);
  return;
}
template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
vector<ValueType> bpTree<KeyType, ValueType, ValueIndexType, HashFunc,
                          IndexCompare, KeyCompare, IndexLess>::FindAll() {
  vector<ValueType> ans;
  if (!root) return ans;
  Node now_node(root);
  LoadNode(now_node);
  while (!now_node.is_leaf) {
    now_node.self = now_node.children[0];
    LoadNode(now_node);
  }
  while (true) {
    for (int pos = 0; pos < now_node.size; pos++)
      if (now_node.ctx[pos]) ans.push_back(GetValue(now_node.ctx[pos]));
    if (!now_node.nxt) return ans;
    now_node.self = now_node.nxt;
    LoadNode(now_node);
  }
}
#endif  // BOOKSTORE_SRC_BPTREE_INL