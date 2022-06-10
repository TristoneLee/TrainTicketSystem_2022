/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <cstddef>
#include <functional>
#include <iostream>

#include "exceptions.hpp"
#include "utility.hpp"

namespace sjtu {

template <class Key, class T, class Compare = std::less<Key> >
class map {
 public:
  /**
   * the internal type of data.
   * it should have a default constructor, a copy constructor.
   * You can use sjtu::map as value_type by typedef.
   */
  typedef pair<const Key, T> value_type;

 private:
  enum Color { Black, Red };
  struct Node {
    Node *parent;
    Node *left_child;
    Node *right_child;
    Node *last;
    Node *next;
    Color color;
    value_type *const val;
    // val is set const to ensure that a node AlWAYS points to the same key
    // so that the iterator pointing to it also points to the same key FOREVER
    Node(value_type *ptr, Color _color = Red)
        : color(_color),
          val(ptr),
          parent(nullptr),
          left_child(nullptr),
          right_child(nullptr),
          last(nullptr),
          next(nullptr) {}
    Node(const value_type &ptr) : Node(new value_type(ptr)) {}
    Node(const Key &key) : Node(new value_type(key, T())) {}
    Node() : Node(nullptr, Black) {}
    Node(const Node &other)
        : Node(other.val ? new value_type(*other.val) : nullptr, other.color) {}
    bool IsLeftChild() { return parent->left_child == this; }
    bool IsRightChild() { return parent->right_child == this; }
    Node *GetBrother() {
      if (!parent) return nullptr;
      if (IsLeftChild()) return parent->right_child;
      return parent->left_child;
    }
    Node *GetUncle() {
      if (!parent) return nullptr;
      return parent->GetBrother();
    }
    Node *GetGrandparent() {
      if (!parent) return nullptr;
      return parent->parent;
    }
    ~Node() {
      if (val) delete val;
    }
  };
  const Compare compare_ = Compare();
  Node *root_;
  // root_ is a void pointer whose left_child is the actual root
  // this can simplify the code because when treating a node,
  // we need not to check if it's the root.
  Node *head_;
  Node *tail_;
  // use a double linked list to implement the iterator ++ and --
  // like root_, head_ and tail_ are also void pointers
  size_t size_;
  Color GetColor(Node *node) {
    // return Black if node is nullptr
    // which means node is a "NIL" node
    if (!node) return Black;
    return node->color;
  }
  bool Equal_(const Key &lhs, const Key &rhs) const {
    // use compare_ to check whether lhs equals to rhs
    return !(compare_(lhs, rhs) || compare_(rhs, lhs));
  }
  Node *DFSCopy_(Node *now_node) {
    if (!now_node) return nullptr;
    Node *new_node = new Node(*now_node);
    new_node->left_child = DFSCopy_(now_node->left_child);
    new_node->right_child = DFSCopy_(now_node->right_child);
    if (new_node->left_child) new_node->left_child->parent = new_node;
    if (new_node->right_child) new_node->right_child->parent = new_node;
    return new_node;
  }

  void DFSBuildLink_(Node *now_node, Node *&last_node) {
    // used after DFSCopy_
    // we can't simply link the new nodes again
    // because we don't know what the orignal "last" and "next" is
    // in the new tree
    // we have to scan the whole tree to build the double linked list again

    // last_node is the last node that we have put into the double linked list
    if (!now_node) return;
    DFSBuildLink_(now_node->left_child, last_node);
    now_node->last = last_node;
    last_node->next = now_node;
    last_node = now_node;
    DFSBuildLink_(now_node->right_child, last_node);
    return;
  }

  void BuildLink_() {
    Node *lst = head_;
    DFSBuildLink_(root_->left_child, lst);
    lst->next = tail_;
    tail_->last = lst;
    return;
  }

  void Copy_(const map &other) {
    size_ = other.size_;
    root_ = DFSCopy_(other.root_);
    BuildLink_();
    return;
  }

  void DFSClear_(Node *now_node) {
    if (!now_node) return;
    DFSClear_(now_node->left_child);
    DFSClear_(now_node->right_child);
    delete now_node;
    return;
  }

  void Clear_() {
    DFSClear_(root_);
    head_->next = tail_;
    tail_->last = head_;
    size_ = 0;
    root_ = nullptr;
    return;
  }
  void LeftRotate_(Node *now_node) {
    Node *parent = now_node->parent;
    Node *original_right_child = now_node->right_child;
    Node *original_right_left_child = original_right_child->left_child;

    if (parent) {
      if (now_node->IsLeftChild())
        parent->left_child = original_right_child;
      else
        parent->right_child = original_right_child;
    }
    original_right_child->parent = parent;

    original_right_child->left_child = now_node;
    now_node->parent = original_right_child;

    now_node->right_child = original_right_left_child;
    if (original_right_left_child) original_right_left_child->parent = now_node;

    return;
  }
  void RightRotate_(Node *now_node) {
    Node *parent = now_node->parent;
    Node *original_left_child = now_node->left_child;
    Node *original_left_right_child = original_left_child->right_child;

    if (parent) {
      if (now_node->IsLeftChild())
        parent->left_child = original_left_child;
      else
        parent->right_child = original_left_child;
    }
    original_left_child->parent = parent;

    original_left_child->right_child = now_node;
    now_node->parent = original_left_child;

    now_node->left_child = original_left_right_child;
    if (original_left_right_child) original_left_right_child->parent = now_node;

    return;
  }
  void InsertBuildLink_(Node *const tar_node) {
    auto SetLast = [&](Node *last_node) -> void {
      // used when we find the last node of tar_node
      // to insert the tar_node into the double linked list (after last_node)
      Node *next_node = last_node->next;
      tar_node->last = last_node;
      tar_node->next = next_node;
      last_node->next = tar_node;
      next_node->last = tar_node;
      return;
    };
    Node *now_node = tar_node;
    // Find the last node
    while (now_node->IsLeftChild()) {
      if (now_node == root_->left_child) {
        // can't find the last node in the whole tree
        // which means the tar_node is the minimum node
        // so insert it after the head_
        SetLast(head_);
        return;
      }
      now_node = now_node->parent;
    }
    // now_node is the right child of its parent
    // which means the tar_node < ... < now_node, all the ... > now_node->parent
    // so now_node->parent is the last node
    SetLast(now_node->parent);
    return;
  }
  void InsertFix_(Node *now_node) {
    while (GetColor(now_node->parent) == Red) {
      Node *uncle = now_node->GetUncle();
      Node *parent = now_node->parent;
      Node *grandparent = parent->parent;
      if (parent->IsLeftChild()) {
        if (GetColor(uncle) == Red) {  // case 1
          parent->color = Black;
          uncle->color = Black;
          grandparent->color = Red;
          now_node = grandparent;
        } else if (GetColor(uncle) == Black &&
                   now_node->IsRightChild()) {  // case 2
          now_node = parent;
          LeftRotate_(now_node);
        } else if (GetColor(uncle) == Black && now_node->IsLeftChild()) {
          // case 3
          parent->color = Black;
          grandparent->color = Red;
          RightRotate_(grandparent);
        }
      } else {
        if (GetColor(uncle) == Red) {  // case 1
          parent->color = Black;
          uncle->color = Black;
          grandparent->color = Red;
          now_node = grandparent;
        } else if (GetColor(uncle) == Black &&
                   now_node->IsLeftChild()) {  // case 2
          now_node = parent;
          RightRotate_(now_node);
        } else if (GetColor(uncle) == Black &&
                   now_node->IsRightChild()) {  // case 3
          parent->color = Black;
          grandparent->color = Red;
          LeftRotate_(grandparent);
        }
      }
    }
    // ensure the root is black
    root_->left_child->color = Black;
    return;
  }

  void Insert_(Node *obj) {
    Node *now_node = root_->left_child;
    Node *now_parent = root_;
    while (now_node) {
      if (compare_(obj->val->first, now_node->val->first)) {
        now_parent = now_node;
        now_node = now_node->left_child;
      } else {
        now_parent = now_node;
        now_node = now_node->right_child;
      }
    }
    if (now_parent == root_ ||
        compare_(obj->val->first, now_parent->val->first))
      now_parent->left_child = obj;
    else
      now_parent->right_child = obj;
    obj->parent = now_parent;
    InsertBuildLink_(obj);
    InsertFix_(obj);
    return;
  }
  void EraseLink_(Node *tar_node) {
    Node *last_node = tar_node->last;
    Node *next_node = tar_node->next;
    last_node->next = next_node;
    next_node->last = last_node;
    return;
  }
  void EraseFix_(Node *now_node) {
    while (now_node != root_->left_child && GetColor(now_node) == Black) {
      Node *brother = now_node->GetBrother();
      Node *parent = now_node->parent;
      if (now_node->IsLeftChild()) {  // case 1
        if (GetColor(brother) == Red) {
          parent->color = Red;
          brother->color = Black;
          LeftRotate_(parent);
        } else if (GetColor(brother) == Black &&
                   GetColor(brother->left_child) == Black &&
                   GetColor(brother->right_child) == Black) {  // case 2
          brother->color = Red;
          now_node = parent;
        } else if (GetColor(brother) == Black &&
                   GetColor(brother->left_child) == Red &&
                   GetColor(brother->right_child) == Black) {  // case3
          if (brother->left_child) brother->left_child->color = Black;
          brother->color = Red;
          RightRotate_(brother);
        } else if (GetColor(brother) == Black &&
                   GetColor(brother->right_child) == Red) {  // case 4
          brother->color = parent->color;
          parent->color = Black;
          if (brother->right_child) brother->right_child->color = Black;
          LeftRotate_(parent);
          break;
        }
      } else {
        if (GetColor(brother) == Red) {  // case 1
          parent->color = Red;
          brother->color = Black;
          RightRotate_(parent);
        } else if (GetColor(brother) == Black &&
                   GetColor(brother->left_child) == Black &&
                   GetColor(brother->right_child) == Black) {  // case 2
          brother->color = Red;
          now_node = parent;
        } else if (GetColor(brother) == Black &&
                   GetColor(brother->right_child) == Red &&
                   GetColor(brother->left_child) == Black) {  // case 3

          if (brother->right_child) brother->right_child->color = Black;
          brother->color = Red;
          LeftRotate_(brother);
        } else if (GetColor(brother) == Black &&
                   GetColor(brother->left_child) == Red) {  // case 4
          brother->color = parent->color;
          parent->color = Black;
          if (brother->left_child) brother->left_child->color = Black;
          RightRotate_(parent);
          break;
        }
      }
    }
    now_node->color = Black;
    return;
  }

  void SwapVal_(Node *a, Node *b) {
    // actually swap the POSTION of a and b on the RbTree
    // and then swap the color of a and b back
    // so that the iterator pointing to a and b will remain valid
    std::swap(a->color, b->color);
    // if a is the father or child of b , they should be treated more CAREFULLY
    if (b->parent == a) std::swap(a, b);
    if (a->parent == b) {
      bool a_is_left = a->IsLeftChild();
      bool b_is_left = b->IsLeftChild();
      Node *a_parent = a->parent;
      Node *b_parent = b->parent;
      Node *a_left_child = a->left_child;
      Node *a_right_child = a->right_child;
      Node *b_left_child = b->left_child;
      Node *b_right_child = b->right_child;
      a->parent = b_parent;
      if (b_is_left)
        b_parent->left_child = a;
      else
        b_parent->right_child = a;
      b->parent = a;
      b->left_child = a_left_child;
      if (a_left_child) a_left_child->parent = b;
      b->right_child = a_right_child;
      if (a_right_child) a_right_child->parent = b;
      if (a_is_left) {
        a->left_child = b;
        a->right_child = b_right_child;
        if (b_right_child) b_right_child->parent = a;
      } else {
        a->right_child = b;
        a->left_child = b_left_child;
        if (b_left_child) b_left_child->parent = a;
      }
      return;
    }
    Node *a_parent = a->parent;
    Node *b_parent = b->parent;
    Node *a_left_child = a->left_child;
    Node *a_right_child = a->right_child;
    Node *b_left_child = b->left_child;
    Node *b_right_child = b->right_child;
    bool a_is_left = a->IsLeftChild();
    bool b_is_left = b->IsLeftChild();
    a->parent = b_parent;
    if (b_is_left)
      b_parent->left_child = a;
    else
      b_parent->right_child = a;

    a->left_child = b_left_child;
    if (b_left_child) b_left_child->parent = a;

    a->right_child = b_right_child;
    if (b_right_child) b_right_child->parent = a;

    b->parent = a_parent;
    if (a_is_left)
      a_parent->left_child = b;
    else
      a_parent->right_child = b;

    b->left_child = a_left_child;
    if (a_left_child) a_left_child->parent = b;

    b->right_child = a_right_child;
    if (a_right_child) a_right_child->parent = b;

    return;
  }
  Node *GetMax_(Node *now_node) {
    // get the maximum node in the subtree of now_node
    while (now_node->right_child) now_node = now_node->right_child;
    return now_node;
  }
  void Erase_(Node *tar_node) {
    EraseLink_(tar_node);
    if (tar_node->color == Red &&
        (!tar_node->left_child || !tar_node->right_child)) {
      // if the node is red and has less than 2 children
      // just delete it without fixing
      // just use its child (if exist) to replace it
      Node *parent = tar_node->parent;
      Node *children =
          tar_node->left_child ? tar_node->left_child : tar_node->right_child;
      if (tar_node->IsLeftChild())
        parent->left_child = children;
      else
        parent->right_child = children;
      if (children) children->parent = parent;
      delete tar_node;
      return;
    }
    if (tar_node->left_child && tar_node->right_child) {
      // if the node has 2 children
      // find the minimum node in the right subtree
      // and swap the value of tar_node and the minimum node
      // then the tar_node has less than 2 children
      // and can be deleted
      Node *replace_node = GetMax_(tar_node->left_child);
      SwapVal_(tar_node, replace_node);
      // actually we just changed the POSTION of tar_node and replace_node
      // so the tar_node still points to the node needs to be deleted
      // so we don't need assign replace_node to tar_node
    }
    Node *child =
        tar_node->left_child ? tar_node->left_child : tar_node->right_child;
    Node *parent = tar_node->parent;
    bool is_left = tar_node->IsLeftChild();
    bool child_is_null = false;
    if (!child) {
      // if child is nullptr, which means it's a "NIL" node
      // as we may use it in EraseFix_
      // just new a temporary node serving as the "NIL" node
      child = new Node(nullptr, Black);
      child_is_null = true;
    }
    if (is_left)
      parent->left_child = child;
    else
      parent->right_child = child;
    child->parent = parent;
    if (tar_node->color == Black) EraseFix_(child);
    delete tar_node;
    if (child_is_null) {
      // if the child is a temporary "NIL" node
      // delete it
      delete child;
      if (is_left)
        parent->left_child = nullptr;
      else
        parent->right_child = nullptr;
    }
    return;
  }
  /* void DFSPrint_(Node *now_node) {
    if (now_node == nullptr) return;
    std::cout << now_node->val->first
              << "[shape = circle , style = filled , fillcolor=";
    if (now_node->color == Black)
      std::cout << "white";
    else
      std::cout << "red";
    std::cout << "]" << std::endl;
    if (now_node->left_child)
      std::cout << now_node->val->first.val << "->"
                << now_node->left_child->val->first.val << "[label=\"L\"];"
                << std::endl;
    if (now_node->right_child)
      std::cout << now_node->val->first.val << "->"
                << now_node->right_child->val->first.val << "[label=\"R\"];"
                << std::endl;
    DFSPrint_(now_node->left_child);
    DFSPrint_(now_node->right_child);
    return;
  } */

 public:
  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
  class const_iterator;
  class iterator {
    friend class map;

   private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    Node *pos_;
    map *parent_;

   public:
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a
    // type. For instance, for an iterator, iterator::value_type is the type
    // that the iterator points to. STL algorithms and containers may use
    // these type_traits (e.g. the following typedef) to work properly. See
    // these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits About value_type:
    // https://blog.csdn.net/u014299153/article/details/72419713 About
    // iterator_category: https://en.cppreference.com/w/cpp/iterator
    using difference_type = std::ptrdiff_t;
    using value_type = map::value_type;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = std::output_iterator_tag;
    // If you are interested in type_traits, toy_traits_test provides a place
    // to practice. But the method used in that test is old and rarely used,
    // so you may explore on your own. Notice: you may add some code in here
    // and class const_iterator and namespace sjtu to implement
    // toy_traits_test, this part is only for bonus.

    iterator() : pos_(nullptr), parent_(nullptr) {}
    iterator(const iterator &other)
        : pos_(other.pos_), parent_(other.parent_) {}
    iterator(Node *pos, map *parent) : pos_(pos), parent_(parent) {}
    /**
     * TODO iter++
     */
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    /**
     * TODO ++iter
     */
    iterator &operator++() {
      if (pos_->next)
        pos_ = pos_->next;
      else
        throw invalid_iterator();
      return *this;
    }
    /**
     * TODO iter--
     */
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }
    /**
     * TODO --iter
     */
    iterator &operator--() {
      if (pos_->last != parent_->head_)
        pos_ = pos_->last;
      else
        throw invalid_iterator();
      return *this;
    }
    /**
     * a operator to check whether two iterators are same (pointing to the
     * same memory).
     */
    value_type &operator*() const {
      if (!(pos_ && pos_->val)) throw invalid_iterator();
      return *(pos_->val);
    }
    bool operator==(const iterator &rhs) const { return pos_ == rhs.pos_; }
    bool operator==(const const_iterator &rhs) const {
      return pos_ == rhs.pos_;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }

    /**
     * for the support of it->first.
     * See
     * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
     * for help.
     */
    value_type *operator->() const noexcept { return pos_->val; }
  };
  class const_iterator {
    friend class map;
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
   private:
    // data members.
    const Node *pos_;
    const map *parent_;

   public:
    using difference_type = std::ptrdiff_t;
    using value_type = const map::value_type;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = std::output_iterator_tag;
    const_iterator() : pos_(nullptr), parent_(nullptr) {}
    const_iterator(const const_iterator &other)
        : pos_(other.pos_), parent_(other.parent_) {}
    const_iterator(const iterator &other)
        : pos_(other.pos_), parent_(other.parent_) {}
    const_iterator(const Node *pos, const map *parent)
        : pos_(pos), parent_(parent) {}

    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      if (pos_->next)
        pos_ = pos_->next;
      else
        throw invalid_iterator();
      return *this;
    }
    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    /**+
     * TODO --iter
     */
    const_iterator &operator--() {
      if (pos_->last != parent_->head_)
        pos_ = pos_->last;
      else
        throw invalid_iterator();
      return *this;
    }

    bool operator==(const iterator &rhs) const { return pos_ == rhs.pos_; }
    bool operator==(const const_iterator &rhs) const {
      return pos_ == rhs.pos_;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }

    const value_type &operator*() const {
      if (!(pos_ && pos_->val)) throw invalid_iterator();
      return *(pos_->val);
    }
    const value_type *operator->() const noexcept { return pos_->val; }
  };
  /**
   * TODO two constructors
   */
  map() : size_(0), root_(new Node()), head_(new Node()), tail_(new Node()) {
    head_->next = tail_;
    tail_->last = head_;
    return;
  }
  map(const map &other) : head_(new Node()), tail_(new Node()) {
    head_->next = tail_;
    tail_->last = head_;
    Copy_(other);
    return;
  }
  /**
   * TODO assignment operator
   */
  map &operator=(const map &other) {
    if (this == &other) return *this;
    Clear_();
    Copy_(other);
    return *this;
  }
  /**
   * TODO Destructors
   */
  ~map() {
    Clear_();
    delete head_;
    delete tail_;
  }
  /**
   * TODO
   * access specified element with bounds checking
   * Returns a reference to the mapped value of the element with key
   * equivalent to key. If no such element exists, an exception of type
   * `index_out_of_bound'
   */
  T &at(const Key &key) {
    auto iter = find(key);
    if (iter == end()) throw index_out_of_bound();
    return iter->second;
  }
  const T &at(const Key &key) const {
    auto iter = find(key);
    if (iter == cend()) throw index_out_of_bound();
    return iter->second;
  }
  /**
   * TODO
   * access specified element
   * Returns a reference to the value that is mapped to a key equivalent to
   * key, performing an insertion if such key does not already exist.
   */
  T &operator[](const Key &key) {
    return (insert(value_type(key, T())).first)->second;
  }
  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T &operator[](const Key &key) const { return at(key); }
  /**
   * return a iterator to the beginning
   */
  iterator begin() { return iterator(head_->next, this); }
  const_iterator cbegin() const { return const_iterator(head_->next, this); }
  /**
   * return a iterator to the end
   * in fact, it returns past-the-end.
   */
  iterator end() { return iterator(tail_, this); }
  const_iterator cend() const { return const_iterator(tail_, this); }
  /**
   * checks whether the container is empty
   * return true if empty, otherwise false.
   */
  bool empty() const { return size_ == 0; }
  /**
   * returns the number of elements.
   */
  size_t size() const { return size_; }
  /**
   * clears the contents
   */
  void clear() {
    Clear_();
    root_ = new Node();
  }
  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the
   * insertion), the second one is true if insert successfully, or false.
   */
  pair<iterator, bool> insert(const value_type &value) {
    auto tmp_it = find(value.first);
    if (find(value.first) != end()) return pair<iterator, bool>(tmp_it, false);
    Node *tmp_node = new Node(value);
    Insert_(tmp_node);
    ++size_;
    return pair<iterator, bool>(iterator(tmp_node, this), true);
  }
  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points
   * an element out of this)
   */
  void erase(iterator pos) {
    if (pos.parent_ != this || pos == end()) throw invalid_iterator();
    --size_;
    Erase_(pos.pos_);
    return;
  }
  /**
   * Returns the number of elements with key
   *   that compares equivalent to the specified argument,
   *   which is either 1 or 0
   *     since this container does not allow duplicates.
   * The default method of check the equivalence is !(a < b || b > a)
   */
  size_t count(const Key &key) const {
    try {
      at(key);
    } catch (index_out_of_bound) {
      return 0;
    }
    return 1;
  }
  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is
   * returned.
   */
  iterator find(const Key &key) {
    Node *now_node = root_->left_child;
    while (now_node) {
      if (Equal_(now_node->val->first, key)) return iterator(now_node, this);
      if (compare_(key, now_node->val->first))
        now_node = now_node->left_child;
      else
        now_node = now_node->right_child;
    }
    return end();
  }
  const_iterator find(const Key &key) const {
    Node *now_node = root_->left_child;
    while (now_node) {
      if (Equal_(now_node->val->first, key))
        return const_iterator(now_node, this);
      if (compare_(key, now_node->val->first))
        now_node = now_node->left_child;
      else
        now_node = now_node->right_child;
    }
    return cend();
  } /*
 void Print_() {
 std::cout << "digraph prim {" << std::endl;
 DFSPrint_(root_->left_child);
 std::cout << "}" << std::endl;
 return;
 } */
};  // namespace sjtu

}  // namespace sjtu

#endif
