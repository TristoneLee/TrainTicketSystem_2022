#ifndef TICKET_SYSTEM_DATABASE_H
#define TICKET_SYSTEM_DATABASE_H

#include <fstream>
#include <iostream>
#include <utility>
#include "vector.h"

using std::fstream;
using std::lower_bound;
using sjtu::vector;
using std::cout;
using std::cin;

namespace sjtu {
    template<class Type>
    class iofile {
    private:
        fstream file;
        std::string file_name;
        int sizeofT = sizeof(Type);
        int count = 0;
        vector<int> availablePlace;
    public:
        iofile() = default;

        explicit iofile(const std::string &_file_name) : file_name(_file_name) {
            file.open(file_name);
            if (file) {
                file.seekg(0);
                file.read(reinterpret_cast<char *>(&count), sizeof(int));
                file.close();
            } else {
                file.clear();
                file.open(file_name, std::fstream::out);
                file.seekp(0);
                file.write(reinterpret_cast<char *>(&count), sizeof(int));
                file.close();
            }
        }


        //在文件合适位置写入类对象t，并返回写入的位置索引index
        //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
        //位置索引index可以取为对象写入的起始位置
        int write(Type &t) {
            file.open(file_name);
            int tmp;
            if (availablePlace.empty()) {
                tmp = (count++) * sizeof(Type) + sizeof(int);
            } else {
                tmp = availablePlace.back();
                availablePlace.pop_back();
            }
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&count), sizeof(int));
            file.seekp(tmp);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
            return tmp;
        }

        void update(Type &t, const int index) {
            file.open(file_name);
            file.seekp(index);
            file.write(reinterpret_cast<char *>(&t), sizeof(Type));
            file.close();
        }

        //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
        void read(Type &t, const int index) {
            file.open(file_name);
            file.seekg(index);
            file.read(reinterpret_cast<char *>(&t), sizeof(Type));
            file.close();
        }

        //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
        void Delete(int index) {
            availablePlace.push_back(index);
        }

        int countOf() { return count; }
    };


    template<class Key, class T, class Compare=std::less<Key>>
    class bpTree {
        friend class Pair;

    public:
        class Pair {
        protected:
            Key key;
            T value;

        public:
            Pair() = default;

            Pair(const Key _key, const T &_value) : key(_key), value(_value) {};

            Key keyOf() const { return key; }

            T valueOf() const { return value; }

            bool operator==(const Pair &rhs) const {
                if (compare(key, rhs.key) || compare(rhs.key, key)) return false;
                else return value == rhs.value;
            }

            bool operator!=(const Pair &rhs) const {
                return !(*this == rhs);
            }

            bool operator<(const Pair &rhs) const {
                if (compare(key, rhs.key))return true;
                else if (compare(rhs.key, key)) return false;
                else return value < rhs.value;
            }

            bool operator>(const Pair &rhs) const {
                if (compare(key, rhs.key))return false;
                else if (compare(rhs.key, key)) return true;
                else return value > rhs.value;
            }

            bool operator<=(const Pair &rhs) const {
                return !(*this > rhs);
            }

            bool operator>=(const Pair &rhs) const {
                return !(*this < rhs);
            }
        };

    private:
        static const int DEGREE = 20;
        static const int MIN_KEY = DEGREE - 1;
        static const int MAX_KEY = 2 * DEGREE - 1;
        static const int MIN_CHILD = DEGREE;
        static const int MAX_CHILD = 2 * DEGREE;
        static const int MAX_DATA = 20;
        static const int MIN_DATA = 10;

        class bpNode {
            friend class bpTree;

            bool isLeaf; //是否是叶节点
            int nodeSiz; //节点内key数量
            int parent; //父节点地址
            int loc; //在document中的地址
            Pair indexes[MAX_KEY];
            int children[MAX_CHILD]{};

            bpNode();

        };

        class array {
            friend class bpTree<Key, T, Compare>;

        private:
            int arraySiz;
            Pair data[MAX_DATA];
            int next;

            array();

            array(Pair, int);

        };

        bpNode root;
        iofile<array> arrayDocument;
        iofile<bpNode> nodeDocument;
        fstream basicData;
        int siz;
        int head;
        int locOfRoot;
        static Compare compare;


        void arraySplit(bpNode &curNode, array &curArray, int &posInNode);

        void nodeSplit(bpNode &curNode);

        void arrayMerge(array &curArray, array &nxtArray, bpNode &curNode, int posInNode);

        void nodeMerge(bpNode &curNode, bpNode &nxtNode, bpNode &faNode, int posInNode);

        void arrayAdoption(bpNode &curNode, array &curArray, int &posInNode);

        void nodeAdoption(bpNode &curNode);

        void indexUpdate(bpNode curNode, const Pair &indexToUpdate);

        int binarySearch(const int *a, int l, int r, const int &obj);

        int binarySearch(const Pair *a, int l, int r, const Pair &obj);

        int binarySearch(const Pair *a, int l, int r, const Key &obj);

        int keySearch(const Pair *a, int l, int r, const Key &obj);

        int keySearch(const Pair *a, int l, int r, const Pair &obj);


    public:
        bpTree();

        ~bpTree();

        sjtu::vector<T> query(Key key);

        bool insert(Pair obj);

        bool erase(Pair obj);

        int size();

        void clear();

    };

    template<class Key, class T, class Compare>
    Compare bpTree<Key, T, Compare>::compare = Compare();

    template<class Key, class T, class Compare>
    bpTree<Key, T, Compare>::bpNode::bpNode() {
        isLeaf = true;
        nodeSiz = 0;
        parent = 0;
        memset(children, 0, sizeof(children));
        loc = 0;
        for (auto &index: indexes) index = Pair();
    }

    template<class Key, class T, class Compare>
    bpTree<Key, T, Compare>::bpTree():root(), nodeDocument("nodeDocument"), arrayDocument("arrayDocument") {
        basicData.open("basicData");
        if (basicData) {
            basicData.seekg(0);
            basicData.read(reinterpret_cast<char *>(&siz), sizeof(int));
            basicData.seekg(sizeof(int));
            basicData.read(reinterpret_cast<char *>(&head), sizeof(int));
            basicData.seekg(2 * sizeof(int));
            basicData.read(reinterpret_cast<char *>(&locOfRoot), sizeof(int));
            basicData.close();
            nodeDocument.read(root, locOfRoot);
        } else {
            basicData.clear();
            basicData.open("basicData", fstream::out);
            basicData.close();
            siz = 0;
            head = 0;
            root.isLeaf = true;
            locOfRoot = root.loc = nodeDocument.write(root);
        }
    }

    template<class Key, class T, class Compare>
    bpTree<Key, T, Compare>::~bpTree() {
        locOfRoot = root.loc;
        basicData.open("basicData");
        basicData.seekp(0);
        basicData.write(reinterpret_cast<char *>(&siz), sizeof(int));
        basicData.seekp(sizeof(int));
        basicData.write(reinterpret_cast<char *>(&head), sizeof(int));
        basicData.seekp(2 * sizeof(int));
        basicData.write(reinterpret_cast<char *>(&locOfRoot), sizeof(int));
        basicData.close();
        nodeDocument.update(root, root.loc);
    }

    template<class Key, class T, class Compare>
    bool bpTree<Key, T, Compare>::insert(Pair obj) {
        if (siz == 0) {
            array tem(obj, 0);
            head = root.children[0] = arrayDocument.write(tem);
            ++siz;
            return true;
        }
        bpNode curNode = root;
        while (!curNode.isLeaf) {
            int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, obj);
            nodeDocument.read(curNode, curNode.children[posInNode]);
        }
        int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, obj);
        array curArray;
        arrayDocument.read(curArray, curNode.children[posInNode]);
        int posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, obj);
        if (curArray.data[posInArray] == obj) return false;
        if (posInArray == 0 && posInNode != 0) curNode.indexes[posInNode - 1] = obj;
        else if (posInNode == 0 && posInArray == 0) indexUpdate(curNode, obj);
        for (int i = curArray.arraySiz; i > posInArray; --i) curArray.data[i] = curArray.data[i - 1];
        curArray.data[posInArray] = obj;
        ++curArray.arraySiz;
        arrayDocument.update(curArray, curNode.children[posInNode]);
        if (curNode.loc != root.loc) nodeDocument.update(curNode, curNode.loc);
        else root = curNode;
        ++siz;
        if (curArray.arraySiz == MAX_DATA) arraySplit(curNode, curArray, posInNode);
        return true;
    }

    template<class Key, class T, class Compare>
    bool bpTree<Key, T, Compare>::erase(Pair obj) {
        if (siz == 0) return false;
        bpNode curNode = root;
        while (!curNode.isLeaf) {
            int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, obj);
            nodeDocument.read(curNode, curNode.children[posInNode]);
        }
        int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, obj);
        array curArray;
        arrayDocument.read(curArray, curNode.children[posInNode]);
        int posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, obj);
        if (curArray.data[posInArray] != obj) return false;
        for (int i = posInArray; i <= curArray.arraySiz - 2; ++i) curArray.data[i] = curArray.data[i + 1];
        if (posInArray == 0 && posInNode != 0) curNode.indexes[posInNode - 1] = curArray.data[0];
        else if (posInNode == 0 && posInArray == 0)indexUpdate(curNode, curArray.data[0]);
        --curArray.arraySiz;
        arrayDocument.update(curArray, curNode.children[posInNode]);
        if (curNode.loc != root.loc) nodeDocument.update(curNode, curNode.loc);
        else root = curNode;
        --siz;
        if (curArray.arraySiz < MIN_DATA) arrayAdoption(curNode, curArray, posInNode);
        return true;
    }

    template<class Key, class T, class Compare>
    int bpTree<Key, T, Compare>::size() { return siz; };

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::clear() {}

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::arraySplit(bpNode &curNode, array &curArray, int &posInNode) {
        array newArray;
        for (int i = MIN_DATA; i < MAX_DATA; ++i) {
            newArray.data[i - MIN_DATA] = curArray.data[i];
        }
        newArray.arraySiz = MAX_DATA - MIN_DATA;
        curArray.arraySiz = MIN_DATA;
        for (int i = curNode.nodeSiz + 1; i > posInNode + 1; --i) curNode.children[i] = curNode.children[i - 1];
        for (int i = curNode.nodeSiz; i > posInNode; --i) curNode.indexes[i] = curNode.indexes[i - 1];
        curNode.indexes[posInNode] = newArray.data[0];
        newArray.next = curArray.next;
        curArray.next = curNode.children[posInNode + 1] = arrayDocument.write(newArray);
        arrayDocument.update(curArray, curNode.children[posInNode]);
        ++curNode.nodeSiz;
        if (curNode.loc == root.loc) root = curNode;
        else (nodeDocument.update(curNode, curNode.loc));
        if (curNode.nodeSiz == MAX_KEY) nodeSplit(curNode);
    }

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::nodeSplit(bpNode &curNode) {
        if (curNode.loc == root.loc) {
            bpNode newNode;
            newNode.isLeaf = curNode.isLeaf = true;
            newNode.nodeSiz = MIN_KEY;
            curNode.nodeSiz = MIN_KEY;
            for (int i = 0; i < MIN_KEY; ++i) newNode.indexes[i] = curNode.indexes[i + MIN_KEY + 1];
            for (int i = 0; i < MIN_CHILD; ++i) newNode.children[i] = curNode.children[i + MIN_CHILD];
            root.nodeSiz = 1;
            root.children[0] = root.loc;
            newNode.loc = root.children[1] = nodeDocument.write(newNode);
            nodeDocument.update(newNode, newNode.loc);
            root.indexes[0] = curNode.indexes[MIN_KEY];
            root.isLeaf = false;
            root.loc = nodeDocument.write(root);
            newNode.parent = curNode.parent = root.loc;
            nodeDocument.update(newNode, newNode.loc);
            nodeDocument.update(curNode, curNode.loc);
        } else {
            bpNode faNode;
            if (curNode.parent == root.loc) faNode = root;
            else nodeDocument.read(faNode, curNode.parent);
            if (faNode.nodeSiz == MAX_KEY) nodeSplit(faNode), nodeSplit(curNode);
            else {
                bpNode newNode;
                newNode.isLeaf = curNode.isLeaf;
                newNode.nodeSiz = MIN_KEY;
                newNode.parent = curNode.parent;
                curNode.nodeSiz = MIN_KEY;
                for (int i = 0; i < MIN_KEY; ++i) newNode.indexes[i] = curNode.indexes[i + MIN_KEY + 1];
                for (int i = 0; i < MIN_CHILD; ++i) newNode.children[i] = curNode.children[i + MIN_CHILD];
                int pos = binarySearch(faNode.children, 0, faNode.nodeSiz + 1, curNode.loc);
                for (int i = faNode.nodeSiz + 1; i > pos + 1; --i) faNode.children[i] = faNode.children[i - 1];
                newNode.loc = faNode.children[pos + 1] = nodeDocument.write(newNode);
                for (int i = faNode.nodeSiz; i > pos; --i) faNode.indexes[i] = faNode.indexes[i - 1];
                ++faNode.nodeSiz;
                faNode.indexes[pos] = curNode.indexes[MIN_KEY];
                nodeDocument.update(newNode, newNode.loc);
                nodeDocument.update(curNode, curNode.loc);
                if (faNode.loc == root.loc) root = faNode;
                else nodeDocument.update(faNode, faNode.loc);
            }
        }
    }

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::arrayMerge(array &curArray, array &nxtArray, bpNode &curNode, int posInNode) {
        for (int i = 0; i < nxtArray.arraySiz; ++i) curArray.data[curArray.arraySiz + i] = nxtArray.data[i];
        curArray.next = nxtArray.next;
        curArray.arraySiz += nxtArray.arraySiz;
        for (int i = posInNode + 1; i < curNode.nodeSiz; ++i) curNode.children[i] = curNode.children[i + 1];
        for (int i = posInNode; i < curNode.nodeSiz - 1; ++i) curNode.indexes[i] = curNode.indexes[i + 1];
        --curNode.nodeSiz;
        arrayDocument.update(curArray, curNode.children[posInNode]);
        if (curNode.loc == root.loc) root = curNode;
        else nodeDocument.update(curNode, curNode.loc);
    }

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::nodeMerge(bpNode &curNode, bpNode &nxtNode, bpNode &faNode, int posInNode) {
        for (int i = 0; i <= nxtNode.nodeSiz; ++i) curNode.children[curNode.nodeSiz + 1 + i] = nxtNode.children[i];
        for (int i = 0; i < nxtNode.nodeSiz; ++i) curNode.indexes[curNode.nodeSiz + i + 1] = nxtNode.indexes[i];
        curNode.indexes[curNode.nodeSiz] = faNode.indexes[posInNode];
        curNode.nodeSiz += nxtNode.nodeSiz;
        for (int i = posInNode; i < faNode.nodeSiz--; ++i) faNode.indexes[i] = faNode.indexes[i + 1];
        for (int i = posInNode + 1; i <= faNode.nodeSiz; ++i) faNode.children[i] = faNode.children[i + 1];
        nodeDocument.update(curNode, curNode.loc);
        nodeDocument.update(nxtNode, nxtNode.loc);
        if (faNode.loc == root.loc) root = faNode;
        else nodeDocument.update(faNode, faNode.loc);
    }

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::arrayAdoption(bpNode &curNode, array &curArray, int &posInNode) {
        if (posInNode == 0) {
            if (posInNode == curNode.nodeSiz) return;
            else {
                array nxtArray;
                arrayDocument.read(nxtArray, curNode.children[posInNode + 1]);
                if (nxtArray.arraySiz > MIN_DATA) {
                    curArray.data[curArray.arraySiz++] = nxtArray.data[0];
                    for (int i = 0; i < nxtArray.arraySiz - 1; ++i) nxtArray.data[i] = nxtArray.data[i + 1];
                    --nxtArray.arraySiz;
                    curNode.indexes[posInNode] = nxtArray.data[0];
                    arrayDocument.update(curArray, curNode.children[posInNode]);
                    arrayDocument.update(nxtArray, curNode.children[posInNode + 1]);
                    if (curNode.loc == root.loc) root = curNode;
                    else nodeDocument.update(curNode, curNode.loc);
                    return;
                } else {
                    arrayMerge(curArray, nxtArray, curNode, posInNode);
                    if (curNode.nodeSiz < MIN_KEY) nodeAdoption(curNode);
                    return;
                }
            }
        } else {
            array preArray;
            arrayDocument.read(preArray, curNode.children[posInNode - 1]);
            if (preArray.arraySiz > MIN_DATA) {
                for (int i = curArray.arraySiz; i > 0; --i) curArray.data[i] = curArray.data[i - 1];
                ++curArray.arraySiz;
                curArray.data[0] = preArray.data[--preArray.arraySiz];
                curNode.indexes[posInNode - 1] = curArray.data[0];
                arrayDocument.update(curArray, curNode.children[posInNode]);
                arrayDocument.update(preArray, curNode.children[posInNode - 1]);
                if (curNode.loc == root.loc) root = curNode;
                else nodeDocument.update(curNode, curNode.loc);
                return;
            } else {
                arrayMerge(preArray, curArray, curNode, posInNode - 1);
                if (curNode.nodeSiz < MIN_KEY) nodeAdoption(curNode);
                return;
            }
        }
    }

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::nodeAdoption(bpNode &curNode) {
        if (curNode.loc == root.loc) return;
        bpNode faNode;
        nodeDocument.read(faNode, curNode.parent);
        int posInNode = binarySearch(faNode.children, 0, faNode.nodeSiz + 1, curNode.loc);
        if (posInNode == 0) {
            if (posInNode == faNode.nodeSiz) return;
            else {
                bpNode nxtNode;
                nodeDocument.read(nxtNode, faNode.children[posInNode + 1]);
                if (nxtNode.nodeSiz > MIN_KEY) {
                    curNode.children[curNode.nodeSiz++] = nxtNode.children[0];
                    for (int i = 0; i < nxtNode.nodeSiz - 1; ++i) nxtNode.children[i] = nxtNode.children[i + 1];
                    --nxtNode.nodeSiz;
                    curNode.indexes[curNode.nodeSiz - 1] = faNode.indexes[posInNode];
                    faNode.indexes[posInNode] = nxtNode.indexes[0];
                    for (int i = 0; i < nxtNode.nodeSiz; ++i) nxtNode.indexes[i] = nxtNode.indexes[i + 1];
                    nodeDocument.update(curNode, curNode.loc);
                    nodeDocument.update(nxtNode, nxtNode.loc);
                    if (faNode.loc == root.loc) root = faNode;
                    else nodeDocument.update(faNode, faNode.loc);
                    return;
                } else {
                    nodeMerge(curNode, nxtNode, faNode, posInNode);
                    if (faNode.nodeSiz < MIN_KEY) nodeAdoption(faNode);
                }
            }
        } else {
            if (posInNode != faNode.nodeSiz) {
                bpNode nxtNode;
                nodeDocument.read(nxtNode, faNode.children[posInNode + 1]);
                if (nxtNode.nodeSiz > MIN_KEY) {
                    curNode.children[curNode.nodeSiz++] = nxtNode.children[0];
                    for (int i = 0; i < nxtNode.nodeSiz - 1; ++i) nxtNode.children[i] = nxtNode.children[i + 1];
                    --nxtNode.nodeSiz;
                    curNode.indexes[curNode.nodeSiz - 1] = faNode.indexes[posInNode];
                    faNode.indexes[posInNode] = nxtNode.indexes[0];
                    for (int i = 0; i < nxtNode.nodeSiz; ++i) nxtNode.indexes[i] = nxtNode.indexes[i + 1];
                    nodeDocument.update(curNode, curNode.loc);
                    nodeDocument.update(nxtNode, nxtNode.loc);
                    if (faNode.loc == root.loc) root = faNode;
                    else nodeDocument.update(faNode, faNode.loc);
                    return;
                }
            } else {
                bpNode preNode;
                nodeDocument.read(preNode, faNode.children[posInNode - 1]);
                if (preNode.nodeSiz > MIN_KEY) {
                    for (int i = curNode.nodeSiz++; i > 0; --i) curNode.children[i] = curNode.children[i - 1];
                    curNode.children[0] = preNode.children[--preNode.nodeSiz];
                    for (int i = curNode.nodeSiz - 1; i > 0; --i) curNode.indexes[i] = curNode.indexes[i - 1];
                    curNode.indexes[0] = faNode.indexes[posInNode - 1];
                    faNode.indexes[posInNode - 1] = preNode.indexes[preNode.nodeSiz];
                    nodeDocument.update(curNode, curNode.loc);
                    nodeDocument.update(preNode, preNode.loc);
                    if (faNode.loc == root.loc) root = faNode;
                    else nodeDocument.update(faNode, faNode.loc);
                    return;
                } else {
                    nodeMerge(preNode, curNode, faNode, posInNode - 1);
                    if (faNode.nodeSiz < MIN_KEY) nodeAdoption(faNode);
                }
            }
        }
    }

    template<class Key, class T, class Compare>
    vector<T> bpTree<Key, T, Compare>::query(Key key) {
        bpNode curNode = root;
        while (!curNode.isLeaf) {
            int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, key);
            nodeDocument.read(curNode, curNode.children[posInNode]);
        }
        int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, key);
        array curArray;
        arrayDocument.read(curArray, curNode.children[posInNode]);
        vector<T> ans;
        bool flag = true;
        int posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, key);
        if (posInArray == curArray.arraySiz)
            arrayDocument.read(curArray, curArray.next),
                    posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, key);
        for (int i = posInArray; i < curArray.arraySiz; ++i) {
            if (curArray.data[i].keyOf() == key) ans.push_back(curArray.data[i].valueOf());
            else {
                flag = false;
                break;
            }
        }
        while (flag) {
            flag = true;
            arrayDocument.read(curArray, curArray.next);
            for (int i = 0; i < curArray.arraySiz; ++i) {
                if (curArray.data[i].keyOf() == key) ans.push_back(curArray.data[i].valueOf());
                else {
                    flag = false;
                    break;
                }
            }
        }
        return ans;
    }

    template<class Key, class T, class Compare>
    void bpTree<Key, T, Compare>::indexUpdate(bpNode curNode, const Pair &indexToUpdate) {
        bpNode faNode;
        nodeDocument.read(faNode, curNode.parent);
        int posInNode = binarySearch(faNode.children, 0, faNode.nodeSiz + 1, curNode.loc);
        while (posInNode == 0 && faNode.loc != root.loc) {
            curNode = faNode;
            nodeDocument.read(faNode, curNode.parent);
            posInNode = binarySearch(faNode.children, 0, faNode.nodeSiz + 1, curNode.loc);
        }
        if (curNode.loc == root.loc && posInNode == 0)return;
        else faNode.indexes[posInNode - 1] = indexToUpdate;
    }

    //闭区间 [l,r],寻找大于等于的第一个
    template<class Key, class T, class Compare>
    int bpTree<Key, T, Compare>::binarySearch(const int *a, int l, int r, const int &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (obj <= a[mid]) r = mid;
            else l = mid + 1;
        }
        return r;
    }

    template<class Key, class T, class Compare>
    int bpTree<Key, T, Compare>::binarySearch(const Pair *a, int l, int r, const Key &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (compare(a[mid].keyOf(), obj)) l = mid + 1;
            else r = mid;
        }
        return r;
    }

    template<class Key, class T, class Compare>
    int bpTree<Key, T, Compare>::binarySearch(const bpTree::Pair *a, int l, int r, const Pair &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (obj <= a[mid]) r = mid;
            else l = mid + 1;
        }
        return r;
    }

    //左闭右开[l,r) 返回r
    template<class Key, class T, class Compare>
    int bpTree<Key, T, Compare>::keySearch(const Pair *a, int l, int r, const Key &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (compare(obj, a[mid].keyOf())) r = mid;
            else l = mid + 1;
        }
        if (r && !compare(a[r - 1].keyOf(), obj) && !compare(obj, a[r - 1].keyOf()))return r - 1;
        else return r;
    }

    template<class Key, class T, class Compare>
    int bpTree<Key, T, Compare>::keySearch(const Pair *a, int l, int r, const Pair &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (a[mid] <= obj) l = mid + 1;
            else r = mid;
        }
        return r;
    }


    template<class Key, class T, class Compare>
    bpTree<Key, T, Compare>::array::array() {
        arraySiz = 0;
        next = 0;
    }

    template<class Key, class T, class Compare>
    bpTree<Key, T, Compare>::array::array(bpTree::Pair obj, int _next) {
        data[0] = obj;
        arraySiz = 1;
        next = _next;
    }

}


#endif //TICKET_SYSTEM_DATABASE_H
