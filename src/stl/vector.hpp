#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template <typename T>
class vector {
 private:
  T **head_;
  size_t now_size_;
  size_t max_size_;
  void clean_space() {
    for (int i = 0; i < now_size_; i++) {
      delete head_[i];
    }
    delete[] head_;
    return;
  }

  void resize() {
    T **new_head = new T *[max_size_];
    for (int i = 0; i < now_size_; i++) new_head[i] = new T((*this)[i]);
    clean_space();
    head_ = new_head;
    return;
  }

  void extend() {
    max_size_ <<= 1;
    resize();
    return;
  }

  void squeeze() {
    if (now_size_ > (max_size_ >> 1)) throw("fatal error : cant squeeze now");
    max_size_ >>= 1;
    resize();
    return;
  }
  void check_squeeze() {
    if (now_size_ <= (max_size_ >> 1)) squeeze();
    return;
  }
  void shift(const size_t &pos) {
    T *tmp_ptr = head_[pos];
    for (int i = pos; i < now_size_ - 1; i++) head_[i] = head_[i + 1];
    head_[now_size_ - 1] = tmp_ptr;
    return;
  }
  void deshift(const size_t &pos) {
    T *tmp_ptr = head_[now_size_ - 1];
    for (int i = now_size_ - 1; i > pos; i--) head_[i] = head_[i - 1];
    head_[pos] = tmp_ptr;
    return;
  }

 public:
  /**
   * TODO
   * a type for actions of the elements of a vector, and you should write
   *   a class named const_iterator with same interfaces.
   */
  /**
   * you can see RandomAccessIterator at CppReference for help.
   */
  class const_iterator;
  class iterator {
    friend class vector;
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a type.
    // For instance, for an iterator, iterator::value_type is the type that the
    // iterator points to.
    // STL algorithms and containers may use these type_traits (e.g. the
    // following typedef) to work properly. In particular, without the following
    // code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type:
    // https://blog.csdn.net/u014299153/article/details/72419713 About
    // iterator_category: https://en.cppreference.com/w/cpp/iterator
   public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

   private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    vector<T> *father_ptr_;
    size_t pos_;

   public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    iterator(vector<T> *father_ptr, const size_t &pos = 0)
        : father_ptr_(father_ptr), pos_(pos) {
      return;
    }
    iterator operator+(const int &n) const {
      return iterator(father_ptr_, pos_ + n);
    }
    iterator operator-(const int &n) const {
      return iterator(father_ptr_, pos_ - n);
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const iterator &rhs) const {
      if (father_ptr_ != rhs.father_ptr_) throw sjtu::invalid_iterator();
      return pos_ - rhs.pos_;
    }
    iterator &operator+=(const int &n) {
      pos_ += n;
      return *this;
    }
    iterator &operator-=(const int &n) {
      pos_ -= n;
      return *this;
    }
    /**
     * TODO iter++
     */
    iterator operator++(int) {
      iterator tmp = *this;
      ++pos_;
      return tmp;
    }
    /**
     * TODO ++iter
     */
    iterator &operator++() {
      ++pos_;
      return (*this);
    }
    /**
     * TODO iter--
     */
    iterator operator--(int) {
      iterator tmp = *this;
      --pos_;
      return tmp;
    }
    /**
     * TODO --iter
     */
    iterator &operator--() {
      --pos_;
      return (*this);
    }
    /**
     * TODO *it
     */
    T &operator*() const { return (*father_ptr_)[pos_]; }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory address).
     */
    bool operator==(const iterator &rhs) const {
      return father_ptr_ == rhs.father_ptr_ && pos_ == rhs.pos_;
    }
    bool operator==(const const_iterator &rhs) const {
      return father_ptr_ == rhs.father_ptr_ && pos_ == rhs.pos_;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };
  /**
   * TODO
   * has same function as iterator, just for a const object.
   */
  class const_iterator {
    friend class vector;

   public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::output_iterator_tag;

   private:
    /*TODO*/

    const vector<T> *father_ptr_;
    size_t pos_;

   public:
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    const_iterator(const vector<T> *father_ptr, const size_t &pos = 0)
        : father_ptr_(father_ptr), pos_(pos) {
      return;
    }
    const_iterator operator+(const int &n) const {
      return iterator(father_ptr_, pos_ + n);
    }
    const_iterator operator-(const int &n) const {
      return iterator(father_ptr_, pos_ - n);
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw
    // invaild_iterator.
    int operator-(const const_iterator &rhs) const {
      if (father_ptr_ != rhs.father_ptr_) throw sjtu::invalid_iterator();
      return pos_ - rhs.pos_;
    }
    const_iterator &operator+=(const int &n) {
      pos_ += n;
      return *this;
    }
    const_iterator &operator-=(const int &n) {
      pos_ -= n;
      return *this;
    }
    /**
     * TODO iter++
     */
    const_iterator operator++(int) {
      iterator tmp = *this;
      ++pos_;
      return tmp;
    }
    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      ++pos_;
      return (*this);
    }
    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      iterator tmp = *this;
      --pos_;
      return tmp;
    }
    /**
     * TODO --iter
     */
    const_iterator &operator--() {
      --pos_;
      return (*this);
    }
    /**
     * TODO *it
     */
    const T &operator*() const { return (*father_ptr_)[pos_]; }
    /**
     * a operator to check whether two iterators are same (pointing to the same
     * memory address).
     */
    bool operator==(const iterator &rhs) const {
      return father_ptr_ == rhs.father_ptr_ && pos_ == rhs.pos_;
    }
    bool operator==(const const_iterator &rhs) const {
      return father_ptr_ == rhs.father_ptr_ && pos_ == rhs.pos_;
    }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return !(*this == rhs); }
    bool operator!=(const const_iterator &rhs) const { return !(*this == rhs); }
  };
  /**
   * TODO Constructs
   * Atleast two: default constructor, copy constructor
   */
  vector() : now_size_(0), max_size_(1), head_(new T *[1]) { return; }
  vector(const vector &other)
      : now_size_(other.now_size_),
        max_size_(other.max_size_),
        head_(new T *[other.max_size_]) {
    for (int i = 0; i < now_size_; i++) head_[i] = new T(other[i]);
    return;
  }
  /**
   * TODO Destructor
   */
  ~vector() {
    clean_space();
    return;
  }
  /**
   * TODO Assignment operator
   */
  vector &operator=(const vector &other) {
    if (this == &other) return *this;
    clean_space();
    max_size_ = other.max_size_;
    now_size_ = other.now_size_;
    head_ = new T *[max_size_];
    for (int i = 0; i < now_size_; i++) head_[i] = new T(other[i]);
    return *this;
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   */
  T &at(const size_t &pos) {
    if (0 <= pos && pos < size()) return *head_[pos];
    throw sjtu::index_out_of_bound();
  }
  const T &at(const size_t &pos) const {
    if (0 <= pos && pos < size()) return *head_[pos];
    throw sjtu::index_out_of_bound();
  }
  /**
   * assigns specified element with bounds checking
   * throw index_out_of_bound if pos is not in [0, size)
   * !!! Pay attentions
   *   In STL this operator does not check the boundary but I want you to do.
   */
  T &operator[](const size_t &pos) {
    if (0 <= pos && pos < size()) return *head_[pos];
    throw sjtu::index_out_of_bound();
  }
  const T &operator[](const size_t &pos) const {
    if (0 <= pos && pos < size()) return *head_[pos];
    throw sjtu::index_out_of_bound();
  }
  /**
   * access the first element.
   * throw container_is_empty if size == 0
   */
  const T &front() const {
    if (empty()) throw sjtu::container_is_empty();
    return (*this)[0];
  }
  /**
   * access the last element.
   * throw container_is_empty if size == 0
   */
  const T &back() const {
    if (empty()) throw sjtu::container_is_empty();
    return (*this)[size() - 1];
  }
  /**
   * returns an iterator to the beginning.
   */
  iterator begin() { return iterator(this, 0); };
  const_iterator cbegin() const { return const_iterator(this, 0); }
  /**
   * returns an iterator to the end.
   */
  iterator end() { return iterator(this, size()); }
  const_iterator cend() const { return const_iterator(this, size()); }
  /**
   * checks whether the container is empty
   */
  bool empty() const { return size() == 0; }
  /**
   * returns the number of elements
   */
  size_t size() const { return now_size_; }
  /**
   * clears the contents
   */
  void clear() {
    clean_space();
    max_size_ = 1;
    now_size_ = 0;
    head_ = new T *[max_size_];
  }
  /**
   * inserts value before pos
   * returns an iterator pointing to the inserted value.
   */
  iterator insert(iterator pos, const T &value) {
    return insert(pos.pos_, value);
  }
  /**
   * inserts value at index ind.
   * after inserting, this->at(ind) == value
   * returns an iterator pointing to the inserted value.
   * throw index_out_of_bound if ind > size (in this situation ind can be size
   * because after inserting the size will increase 1.)
   */
  iterator insert(const size_t &ind, const T &value) {
    if (ind > size()) throw sjtu::index_out_of_bound();
    push_back(value);
    deshift(ind);
    return iterator(this, ind);
  }
  /**
   * removes the element at pos.
   * return an iterator pointing to the following element.
   * If the iterator pos refers the last element, the end() iterator is
   * returned.
   */
  iterator erase(iterator pos) { return erase(pos.pos_); }
  /**
   * removes the element with index ind.
   * return an iterator pointing to the following element.
   * throw index_out_of_bound if ind >= size
   */
  iterator erase(const size_t &ind) {
    if (ind > size()) throw sjtu::index_out_of_bound();
    shift(ind);
    pop_back();
    return iterator(this, ind);
  }
  /**
   * adds an element to the end.
   */
  void push_back(const T &value) {
    if (now_size_ == max_size_) extend();
    head_[now_size_] = new T(value);
    now_size_++;
    return;
  }
  /**
   * remove the last element from the end.other.max_size_
   * throw container_is_empty if size() == 0
   */
  void pop_back() {
    if (empty()) throw sjtu::container_is_empty();
    delete head_[now_size_ - 1];
    --now_size_;
    check_squeeze();
    return;
  }
};

}  // namespace sjtu

#endif
