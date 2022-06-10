#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cmath>

namespace sjtu {

    template<class T>
    class vector {
    private:
        int siz = 0;
        int limit = 100;
        T *data;

        void doubleSpace() {
            limit *= 2;
            auto tmp_data = (T *) malloc(limit * sizeof(T));
            for (int i = 0; i < siz; ++i) {
                new(tmp_data + i) T(data[i]);
                data[i].~T();
            }
            free(data);
            data = tmp_data;
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
            // The following code is written for the C++ type_traits library.
            // Type traits is a C++ feature for describing certain properties of a type.
            // For instance, for an iterator, iterator::value_type is the type that the
            // iterator points to.
            // STL algorithms and containers may use these type_traits (e.g. the following
            // typedef) to work properly. In particular, without the following code,
            // @code{std::sort(iter, iter1);} would not compile.
            // See these websites for more information:
            // https://en.cppreference.com/w/cpp/header/type_traits
            // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
            // About iterator_category: https://en.cppreference.com/w/cpp/iterator
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

            int pos = 0;
            const vector<T> *owner = nullptr;

            iterator(int pos_, const vector<T> *owner_) : pos(pos_), owner(owner_) {};

            iterator(const iterator &otr) : pos(otr.pos), owner(otr.owner) {};

            /**
             * return a new iterator which pointer n-next elements
             * as well as operator-
             */
            iterator operator+(const int &n) const {
                return iterator(pos + n, owner);
            }

            iterator operator-(const int &n) const {
                return iterator(pos - n, owner);
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const iterator &rhs) const {
                if (owner != rhs.owner) throw invalid_iterator();
                else return pos - rhs.pos;
            }

            iterator &operator+=(const int &n) {
                pos += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                pos -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                auto tmp = iterator(*this);
                pos += 1;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                pos += 1;
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                auto tmp = iterator(*this);
                pos -= 1;
                return tmp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                pos -= 1;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return owner->data[pos];
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory address).
             */
            bool operator==(const iterator &rhs) const {
                return pos == rhs.pos && owner == rhs.owner;
            }

            bool operator==(const const_iterator &rhs) const {
                return pos == rhs.pos && owner == rhs.owner;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(pos == rhs.pos && owner == rhs.owner);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(pos == rhs.pos && owner == rhs.owner);
            }
        };

        /**
         * TODO
         * has same function as iterator, just for a const object.
         */
        class const_iterator {
        public:
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T *;
            using reference = T &;
            using iterator_category = std::output_iterator_tag;

            int pos = 0;
            const vector<T> *owner = nullptr;

            const_iterator(int pos_, const vector<T> *owner_) : pos(pos_), owner(owner_) {};

            const_iterator(const iterator &otr) : pos(otr.pos), owner(otr.owner) {};

            const_iterator(const const_iterator &otr) : pos(otr.pos), owner(otr.owner) {};

            const_iterator operator+(const int &n) const {
                return const_iterator(pos + n, owner);
            }

            const_iterator operator-(const int &n) const {
                return const_iterator(pos - n, owner);
            }

            // return the distance between two iterators,
            // if these two iterators point to different vectors, throw invaild_iterator.
            int operator-(const const_iterator &rhs) const {
                if (owner != rhs.owner) throw invalid_iterator();
                else return pos - rhs.pos;
            }

            const_iterator &operator+=(const int &n) {
                pos += n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                pos -= n;
                return *this;
            }

            /**
             * TODO iter++
             */
            const_iterator operator++(int) {
                auto tmp = const_iterator(*this);
                pos += 1;
                return tmp;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                pos += 1;
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                auto tmp = const_iterator(*this);
                pos -= 1;
                return tmp;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                pos -= 1;
                return *this;
            }

            /**
             * TODO *it
             */
            T &operator*() const {
                return owner->data[pos];
            }

            bool operator==(const iterator &rhs) const {
                return pos == rhs.pos && owner == rhs.owner;
            }

            bool operator==(const const_iterator &rhs) const {
                return pos == rhs.pos && owner == rhs.owner;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return !(pos == rhs.pos && owner == rhs.owner);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(pos == rhs.pos && owner == rhs.owner);
            }
        };

        /**
         * TODO Constructs
         * Atleast two: default constructor, copy constructor
         */
        vector() {
            data = (T *) malloc(sizeof(T));
        };

        vector(const vector &other) {
            siz = other.siz;
            limit = other.limit;
            data = (T *) malloc(limit * sizeof(T));
            for (int i = 0; i < siz; ++i) new(data + i) T(other[i]);
        };

        /**
         * TODO Destructor
         */
        ~vector() {
            for (int i = 0; i < siz; ++i) data[i].~T();
            free(data);
            //delete [];
        }

        /**
         * TODO Assignment operator
         */
        vector &operator=(const vector &other) {
            if (&other != this) {
                for (int i = 0; i < siz; ++i) data[i].~T();
                free(data);
                siz = other.siz;
                limit = other.limit;
                data = (T *) malloc(limit * sizeof(T));
                for (int i = 0; i < siz; ++i) new(data + i) T(other[i]);
            }
            return *this;
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, siz)
         */
        T &at(const size_t &pos) {
            if (pos < 0 || pos >= siz) throw index_out_of_bound();
            else return data[pos];
        }

        const T &at(const size_t &pos) const {
            if (pos < 0 || pos >= siz) throw index_out_of_bound();
            else return data[pos];
        }

        /**
         * assigns specified element with bounds checking
         * throw index_out_of_bound if pos is not in [0, siz)
         * !!! Pay attentions
         *   In STL this operator does not check the boundary but I want you to do.
         */
        T &operator[](const size_t &pos) {
            if (pos < 0 || pos >= siz) throw index_out_of_bound();
            else return data[pos];
        }

        const T &operator[](const size_t &pos) const {
            if (pos < 0 || pos >= siz) throw index_out_of_bound();
            else return data[pos];
        }

        /**
         * access the first element.
         * throw container_is_empty if siz == 0
         */
        const T &front() const {
            if (siz == 0) throw container_is_empty();
            else return *data;
        }

        /**
         * access the last element.
         * throw container_is_empty if siz == 0
         */
        const T &back() const {
            if (siz == 0) throw container_is_empty();
            else return data[siz - 1];
        }

        /**
         * returns an iterator to the beginning.
         */
        iterator begin() {
            return iterator(0, this);
        }

        const_iterator cbegin() const {
            return const_iterator(0, this);
        }

        /**
         * returns an iterator to the end.
         */
        iterator end() {
            return iterator(siz, this);
        }

        const_iterator cend() const {
            return const_iterator(siz, this);
        }

        /**
         * checks whether the container is empty
         */
        bool empty() const {
            return siz == 0;
        }

        /**
         * returns the number of elements
         */
        size_t size() const {
            return siz;
        }

        /**
         * clears the contents
         */
        void clear() {
            siz = 0;
        }

        /**
         * inserts value before pos
         * returns an iterator pointing to the inserted value.
         */
        iterator insert(iterator pos_, const T &value) {
            if (siz == limit) doubleSpace();
            for (int i = siz; i > pos_.pos; --i) data[i] = data[i - 1];
            data[pos_.pos] = value;
            ++siz;
            return iterator(pos_.pos, this);
        }

        /**
         * inserts value at index ind.
         * after inserting, this->at(ind) == value
         * returns an iterator pointing to the inserted value.
         * throw index_out_of_bound if ind > siz (in this situation ind can be siz because after inserting the siz will increase 1.)
         */
        iterator insert(const size_t &ind, const T &value) {
            if (ind > siz) throw index_out_of_bound();
            if (siz == limit) doubleSpace();
            for (int i = siz; i > ind; --i) data[i] = data[i - 1];
            data[ind] = value;
            ++siz;
            return iterator(ind, this);
        }

        /**
         * removes the element at pos.
         * return an iterator pointing to the following element.
         * If the iterator pos refers the last element, the end() iterator is returned.
         */
        iterator erase(iterator pos_) {
            data[pos_.pos].~T();
            for (int i = pos_.pos; i < siz - 1; ++i) data[i] = data[i + 1];
            --siz;
            return iterator(pos_.pos, this);
        }

        /**
         * removes the element with index ind.
         * return an iterator pointing to the following element.
         * throw index_out_of_bound if ind >= siz
         */
        iterator erase(const size_t &ind) {
            if (ind >= siz) throw index_out_of_bound();
            data[ind].~T();
            for (int i = ind; i < siz - 1; ++i) data[i] = data[i + 1];
            --siz;
            return iterator(ind, this);
        }

        /**
         * adds an element to the end.
         */
        void push_back(const T &value) {
            if (siz == limit) doubleSpace();
            new(data + siz) T(value);
            ++siz;
        }

        /**
         * remove the last element from the end.
         * throw container_is_empty if siz() == 0
         */
        void pop_back() {
            if (siz == 0) throw container_is_empty();
            data[siz-1].~T();
            --siz;
        }
    };


}

#endif
