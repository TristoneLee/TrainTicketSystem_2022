#ifndef BOOKSTORE_SRC_FILESYSTEM_BPTREE_H
#define BOOKSTORE_SRC_FILESYSTEM_BPTREE_H
#include <functional> 
#include <string>
#include <utility>
#include <vector>

// #include "Array.h"
#include "DefaultConst.hpp"
extern const int max_node_size;
#include "FileMemPool.h"
using std::pair;
using std::string;
using std::vector;

#include "Comp.h"
template <class KeyType, class ValueType, class ValueIndexType = int,
          class HashFunc = Comp::BailanHash<ValueType>,
          class IndexCompare =
              Comp::CompToLess<Comp::PairCompFirst<KeyType, ValueIndexType>,
                               pair<KeyType, ValueIndexType>>,
          class KeyCompare = Comp::SingleComp<KeyType>,
          class IndexLess = std::less<pair<KeyType, ValueIndexType>>>
class FileMap {
  typedef pair<KeyType, ValueIndexType> IndexType;

 private:
  class Node;
  void Init();
  void Load();
  void Save();
  void DFSPrint(FilePtr<Node>);
  Node CreatNode();
  void StoreNode(Node& obj);
  void UpdateNode(const Node& obj);
  void LoadNode(Node& obj) const;
  ValueType GetValue(const FilePtr<ValueType>& ptr);
  void SplitRoot();
  void DFSInsert(const IndexType& tmp_index, const FilePtr<ValueType>& value,
                 Node& now_node);
  void DFSFind(const KeyType& key, Node& now_node, vector<ValueType>&);
  void DFSErase(const IndexType& tmp_index,Node& now_node);
  // void Split(const FilePtr<Node>&);
  FileMemPool node_file;
  FileMemPool ctx_file;
  FilePtr<Node> root;

 public:
  FileMap(const string& _file_name);
  void Print();
  void insert(const KeyType& key, const ValueType& value);
  std::vector<ValueType> query(const KeyType& key);
  std::vector<ValueType> FindAll();
  // ValueType Find(const KeyType& key, const ValueIndexType index);
  void erase(const KeyType& key, const ValueType& value);
};

template <class KeyType, class ValueType, class ValueIndexType, class HashFunc,
          class IndexCompare, class KeyCompare, class IndexLess>
class FileMap<KeyType, ValueType, ValueIndexType, HashFunc, IndexCompare,
              KeyCompare, IndexLess>::Node {
  friend class FileMap<KeyType, ValueType, ValueIndexType, HashFunc,
                       IndexCompare, KeyCompare, IndexLess>;

 private:
  int size;
  bool is_leaf;
  FilePtr<Node> self;
  FilePtr<Node> lst;
  FilePtr<Node> nxt;
  FilePtr<Node> parent;
  FilePtr<Node> children[max_node_size + 1];
  FilePtr<ValueType> ctx[max_node_size + 1];
  IndexType index[max_node_size + 1];
  void InsertCtx(const IndexType& tmp_index, const FilePtr<ValueType>& value,
                 int pos);
  void InsertNode(Node& other, int pos);
  // int LowerBound(const KeyType& key, const ValueType& value);
  int LowerBound(const KeyType& key);
  int LowerBound(const IndexType& tmp_index);

 public:
  Node();
  Node(const FilePtr<Node>& _self);
  void Move(int lpos, int rpos);
  void InsertCtx(const IndexType& tmp_index, const FilePtr<ValueType>& value);
  int InsertNode(Node& other);  // return the postion of the inserted node
  // void BPInsert(const KeyType& key, const ValueType value);
  int Find(const KeyType& key);
  int Find(const KeyType& key, const ValueType value);
  void Erase(int pos);
  void Append(const Node& other);
  void HalfSpilt(Node&);
  void BecomeParent(Node& lchild, Node& rchild);
};
#include "BPTree.inl"
#include "BPTree_Node.inl"
#endif  // BOOKSTORE_SRC_FILESYSTEM_BPTREE_H