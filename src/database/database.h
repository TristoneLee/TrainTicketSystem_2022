#ifndef TICKET_SYSTEM_DATABASE_H
#define TICKET_SYSTEM_DATABASE_H

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include "vector.hpp"
#include "defaultHash.h"
#include "rollback.h"

using std::fstream;
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
        std::vector<int> availablePlace;
    public:
        iofile() = default;

        explicit iofile(const std::string &_file_name) : file_name(_file_name + ".dat") {
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


        ~iofile() {
            file.open(file_name);
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&count), sizeof(int));
            file.close();
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

    template<class Key,
            class Value,
            class HashType=int,
            class HashFunc=defaultHash<Value>,
            class KeyCompare=std::less<Key>,
            class HashCompare=std::less<HashType>>
    class bpTree {
        friend class indexPair;

        friend class iterator;

    public:
        class storagePair {
            friend class bpTree;

        private:
            Key key;
            Value value;
        public:
            storagePair() = default;

            storagePair(const Key &_key, const Value &_value) : key(_key), value(_value) {};

            Key keyOf() const { return key; }

            Value valueOf() const { return value; }
        };

        class indexPair {
            friend class bpTree;

        protected:
            Key key;
            HashType value;
            int pos;

        public:
            indexPair() = default;

            indexPair(const Key _key, const HashType &_value, int _pos) : key(_key), value(_value), pos(_pos) {};

            Key keyOf() const { return key; }

            HashType valueOf() const { return value; }

            int posOf() const { return pos; }

            bool operator==(const indexPair &rhs) const {
                if (keyCompare(key, rhs.key) || keyCompare(rhs.key, key)) return false;
                else return !(hashCompare(value, rhs.value) || hashCompare(rhs.value, value));
            }

            bool operator!=(const indexPair &rhs) const {
                return !(*this == rhs);
            }

            bool operator<(const indexPair &rhs) const {
                if (keyCompare(key, rhs.key))return true;
                else if (keyCompare(rhs.key, key)) return false;
                else return hashCompare(value, rhs.value);
            }

            bool operator>(const indexPair &rhs) const {
                if (keyCompare(key, rhs.key))return false;
                else if (keyCompare(rhs.key, key)) return true;
                else return hashCompare(rhs.value, value);
            }

            bool operator<=(const indexPair &rhs) const {
                return !(*this > rhs);
            }

            bool operator>=(const indexPair &rhs) const {
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
            indexPair indexes[MAX_KEY];
            int children[MAX_CHILD]{};

            bpNode();
        };

        class array {
            friend class bpTree;

        private:
            int arraySiz;
            indexPair data[MAX_DATA];
            int next;

            array();

            array(indexPair, int);

        };

        class iterator {
            friend class bpTree;

        private:
            int pos;
            int num;
            int next;
            int arraySiz;
            bpTree *master;

        public:
            iterator(int _next, int _pos, int _num, int _arraySiz, bpTree *_master) : next(_next), pos(_pos),
                                                                                      arraySiz(_arraySiz),
                                                                                      master(_master), num(_num) {};

            iterator &operator++() {
                ++num;
                if (num == arraySiz) {
                    if (next == 0) {
                        num = -1;
                        return *this;
                    }
                    array tem;
                    master->arrayDocument.read(tem, next);
                    num = 0;
                    pos = next;
                    next = tem.next;
                    arraySiz = tem.arraySiz;
                    return *this;
                }
                return *this;
            }

            bool ifEnd() {
                return num == -1;
            }

            storagePair operator*() {
                array tem;
                master->arrayDocument.read(tem, pos);
                storagePair pair;
                master->storageDocument.read(pair, tem.data[num].pos);
                return pair;
            }
        };

        bpNode root;
        iofile<array> arrayDocument;
        iofile<bpNode> nodeDocument;
        iofile<storagePair> storageDocument;
        fstream basicData;
        int siz;
        int head;
        int locOfRoot;
        static KeyCompare keyCompare;
        static HashCompare hashCompare;
        static HashFunc hashFunc;
        Rollback<Value> rollback;
        std::string filename;


        void arraySplit(bpNode &curNode, array &curArray, int &posInNode);

        void nodeSplit(bpNode &curNode);

        void arrayMerge(array &curArray, array &nxtArray, bpNode &curNode, int posInNode);

        void nodeMerge(bpNode &curNode, bpNode &nxtNode, bpNode &faNode, int posInNode);

        void arrayAdoption(bpNode &curNode, array &curArray, int &posInNode);

        void nodeAdoption(bpNode &curNode);

        void indexUpdate(bpNode curNode, const indexPair &indexToUpdate);

        int binarySearch(const int *a, int l, int r, const int &obj);

        int noOrderSearch(const int *a, int l, int r, const int &obj);

        int binarySearch(const indexPair *a, int l, int r, const indexPair &obj);

        int binarySearch(const indexPair *a, int l, int r, const Key &obj);

        int keySearch(const indexPair *a, int l, int r, const Key &obj);

        int keySearch(const indexPair *a, int l, int r, const indexPair &obj);


    public:
        bpTree(std::string name);

        ~bpTree();

        sjtu::vector<Value> query(Key key);

        int insert(Key key, Value value, int time);

        int rollbackInsert(Key key, Value value, int pos);

        bool erase(Key key, Value value, int time);

        int rollbackErase(Key key, Value value);

        int size();

        void clear();

        void valueUpdate(iterator iter, Value newValue, int time);

        Value dirRead(int pos);

        iterator find(Key key);

        iterator pairFind(Key key, Value value);

        iterator begin();

        void roll();
    };

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    KeyCompare bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::keyCompare = KeyCompare();

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    HashCompare bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::hashCompare = HashCompare();

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    HashFunc bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::hashFunc = HashFunc();

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::bpNode::bpNode() {
        isLeaf = true;
        nodeSiz = 0;
        parent = 0;
        memset(children, 0, sizeof(children));
        loc = 0;
        for (auto &index: indexes) index = indexPair();
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::bpTree(std::string name)
            :root(), nodeDocument(name + "nodeDocument"), arrayDocument(name + "arrayDocument"),
             storageDocument(name + "storageDocument"), rollback(name), filename(name) {
        basicData.open(name + "basicData" + ".dat");
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
            basicData.open(name + "basicData" + ".dat", fstream::out);
            basicData.close();
            siz = 0;
            head = 0;
            root.isLeaf = true;
            locOfRoot = root.loc = nodeDocument.write(root);
        }
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::~bpTree() {
        locOfRoot = root.loc;
        basicData.open(filename + "basicData.dat");
        basicData.seekp(0);
        basicData.write(reinterpret_cast<char *>(&siz), sizeof(int));
        basicData.seekp(sizeof(int));
        basicData.write(reinterpret_cast<char *>(&head), sizeof(int));
        basicData.seekp(2 * sizeof(int));
        basicData.write(reinterpret_cast<char *>(&locOfRoot), sizeof(int));
        basicData.close();
        nodeDocument.update(root, root.loc);
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::insert(Key key, Value value, int time) {
        HashType valueHash = hashFunc(value);
        indexPair obj(key, valueHash, 0);
        if (siz == 0) {
            storagePair pair(key, value);
            int valuePos = storageDocument.write(pair);
            obj.pos = valuePos;
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
        if (curArray.data[posInArray] == obj && posInArray != curArray.arraySiz) return 0;
        storagePair pair(key, value);
        int valuePos = storageDocument.write(pair);
        obj.pos = valuePos;
        if (posInArray == 0 && posInNode != 0) curNode.indexes[posInNode - 1] = obj;
        else if (posInNode == 0 && posInArray == 0 && curNode.loc != root.loc) indexUpdate(curNode, obj);
        for (int i = curArray.arraySiz; i > posInArray; --i) curArray.data[i] = curArray.data[i - 1];
        curArray.data[posInArray] = obj;
        ++curArray.arraySiz;
        arrayDocument.update(curArray, curNode.children[posInNode]);
        if (curNode.loc != root.loc) nodeDocument.update(curNode, curNode.loc);
        else root = curNode;
        ++siz;
        if(time>0) rollback.push(time, 1, valuePos);
        if (curArray.arraySiz == MAX_DATA) arraySplit(curNode, curArray, posInNode);
        return valuePos;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    bool bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::erase(Key key, Value value, int time) {
        HashType valueHash = hashFunc(value);
        indexPair obj(key, valueHash, 0);
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
        if(time>0)rollback.push(time, 2, curNode.children[posInNode]);
        if (curArray.arraySiz < MIN_DATA) arrayAdoption(curNode, curArray, posInNode);
        return true;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::size() { return siz; };

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::clear() {}

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::arraySplit(bpNode &curNode, array &curArray,
                                                                                     int &posInNode) {
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

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::nodeSplit(bpNode &curNode) {
        if (curNode.loc == root.loc) {
            bpNode newNode;
            newNode.isLeaf = curNode.isLeaf;
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
            if (!newNode.isLeaf) {
                bpNode tem;
                for (int i = 0; i <= newNode.nodeSiz; ++i) {
                    nodeDocument.read(tem, newNode.children[i]);
                    tem.parent = newNode.loc;
                    nodeDocument.update(tem, tem.loc);
                }
            }
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
                int pos = noOrderSearch(faNode.children, 0, faNode.nodeSiz, curNode.loc);
                for (int i = faNode.nodeSiz + 1; i > pos + 1; --i) faNode.children[i] = faNode.children[i - 1];
                newNode.loc = faNode.children[pos + 1] = nodeDocument.write(newNode);
                for (int i = faNode.nodeSiz; i > pos; --i) faNode.indexes[i] = faNode.indexes[i - 1];
                ++faNode.nodeSiz;
                faNode.indexes[pos] = curNode.indexes[MIN_KEY];
                nodeDocument.update(newNode, newNode.loc);
                nodeDocument.update(curNode, curNode.loc);
                if (faNode.loc == root.loc) root = faNode;
                else nodeDocument.update(faNode, faNode.loc);
                if (!newNode.isLeaf) {
                    bpNode tem;
                    for (int i = 0; i <= newNode.nodeSiz; ++i) {
                        nodeDocument.read(tem, newNode.children[i]);
                        tem.parent = newNode.loc;
                        nodeDocument.update(tem, tem.loc);
                    }
                }
            }
        }
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::arrayMerge(array &curArray, array &nxtArray,
                                                                                     bpNode &curNode, int posInNode) {
        for (int i = 0; i < nxtArray.arraySiz; ++i) curArray.data[curArray.arraySiz + i] = nxtArray.data[i];
        arrayDocument.Delete(curArray.next);
        curArray.next = nxtArray.next;
        curArray.arraySiz += nxtArray.arraySiz;
        for (int i = posInNode + 1; i < curNode.nodeSiz; ++i) curNode.children[i] = curNode.children[i + 1];
        for (int i = posInNode; i < curNode.nodeSiz - 1; ++i) curNode.indexes[i] = curNode.indexes[i + 1];
        --curNode.nodeSiz;
        arrayDocument.update(curArray, curNode.children[posInNode]);
        if (curNode.loc == root.loc) root = curNode;
        else nodeDocument.update(curNode, curNode.loc);
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::nodeMerge(bpNode &curNode, bpNode &nxtNode,
                                                                                    bpNode &faNode, int posInNode) {
        if (!curNode.isLeaf) {
            bpNode tem;
            for (int i = 0; i <= nxtNode.nodeSiz; ++i) {
                nodeDocument.read(tem, nxtNode.children[i]);
                tem.parent = curNode.loc;
                nodeDocument.update(tem, tem.loc);
            }
        }
        for (int i = 0; i <= nxtNode.nodeSiz; ++i) curNode.children[curNode.nodeSiz + 1 + i] = nxtNode.children[i];
        for (int i = 0; i < nxtNode.nodeSiz; ++i) curNode.indexes[curNode.nodeSiz + i + 1] = nxtNode.indexes[i];
        curNode.indexes[curNode.nodeSiz] = faNode.indexes[posInNode];
        curNode.nodeSiz += nxtNode.nodeSiz + 1;
        for (int i = posInNode; i < faNode.nodeSiz - 1; ++i) faNode.indexes[i] = faNode.indexes[i + 1];
        --faNode.nodeSiz;
        for (int i = posInNode + 1; i <= faNode.nodeSiz; ++i) faNode.children[i] = faNode.children[i + 1];
        nodeDocument.update(curNode, curNode.loc);
        nodeDocument.Delete(nxtNode.loc);
        if (faNode.loc == root.loc) root = faNode;
        else nodeDocument.update(faNode, faNode.loc);
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::arrayAdoption(bpNode &curNode, array &curArray,
                                                                                   int &posInNode) {
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

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::nodeAdoption(bpNode &curNode) {
        if (curNode.loc == root.loc) return;
        bpNode faNode;
        if (curNode.parent == root.loc) faNode = root;
        else nodeDocument.read(faNode, curNode.parent);
        int posInNode = noOrderSearch(faNode.children, 0, faNode.nodeSiz, curNode.loc);
        if (posInNode == 0) {
            if (posInNode == faNode.nodeSiz) return;
            else {
                bpNode nxtNode;
                nodeDocument.read(nxtNode, faNode.children[posInNode + 1]);
                if (nxtNode.nodeSiz > MIN_KEY) {
                    curNode.children[curNode.nodeSiz + 1] = nxtNode.children[0];
                    if (!nxtNode.isLeaf) {
                        bpNode tem;
                        nodeDocument.read(tem, nxtNode.children[0]);
                        tem.parent = curNode.loc;
                        nodeDocument.update(tem, tem.loc);
                    }
                    ++curNode.nodeSiz;
                    for (int i = 0; i < nxtNode.nodeSiz; ++i) nxtNode.children[i] = nxtNode.children[i + 1];
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
                    curNode.children[curNode.nodeSiz + 1] = nxtNode.children[0];
                    if (!nxtNode.isLeaf) {
                        bpNode tem;
                        nodeDocument.read(tem, nxtNode.children[0]);
                        tem.parent = curNode.loc;
                        nodeDocument.update(tem, tem.loc);
                    }
                    ++curNode.nodeSiz;
                    for (int i = 0; i < nxtNode.nodeSiz; ++i) nxtNode.children[i] = nxtNode.children[i + 1];
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
                    bpNode preNode;
                    nodeDocument.read(preNode, faNode.children[posInNode - 1]);
                    if (preNode.nodeSiz > MIN_KEY) {
                        ++curNode.nodeSiz;
                        for (int i = curNode.nodeSiz; i > 0; --i) curNode.children[i] = curNode.children[i - 1];
                        curNode.children[0] = preNode.children[preNode.nodeSiz];
                        if (!curNode.isLeaf) {
                            bpNode tem;
                            nodeDocument.read(tem, curNode.children[0]);
                            tem.parent = curNode.loc;
                            nodeDocument.update(tem, tem.loc);
                        }
                        --preNode.nodeSiz;
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
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    sjtu::vector<Value> bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::query(Key key) {
        sjtu::vector<Value> ans;
        if (siz == 0) return ans;
        bpNode curNode = root;
        while (!curNode.isLeaf) {
            int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, key);
            nodeDocument.read(curNode, curNode.children[posInNode]);
        }
        int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, key);
        array curArray;
        arrayDocument.read(curArray, curNode.children[posInNode]);
        bool flag = true;
        int posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, key);
        if (posInArray == curArray.arraySiz) {
            if (curArray.next != 0) {
                arrayDocument.read(curArray, curArray.next);
                ++posInNode;
                posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, key);
            } else return ans;
        }
        for (int i = posInArray; i < curArray.arraySiz; ++i) {
            storagePair pair;
            if (!keyCompare(curArray.data[i].keyOf(), key) && !keyCompare(key, curArray.data[i].keyOf())) {
//            if (curArray.data[i].keyOf()==key) {
                storageDocument.read(pair, curArray.data[i].pos);
                ans.push_back(pair.valueOf());
            } else {
                flag = false;
                break;
            }
        }
        while (flag && curArray.next) {
            flag = true;
            arrayDocument.read(curArray, curArray.next);
            storagePair pair;
            for (int i = 0; i < curArray.arraySiz; ++i) {
                if (!keyCompare(curArray.data[i].keyOf(), key) && !keyCompare(key, curArray.data[i].keyOf())) {
//                if (curArray.data[i].keyOf()==key) {
                    storageDocument.read(pair, curArray.data[i].pos);
                    ans.push_back(pair.valueOf());
                } else {
                    flag = false;
                    break;
                }
            }
        }
        return ans;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::indexUpdate(bpNode curNode,
                                                                                      const indexPair &indexToUpdate) {
        if (curNode.loc == root.loc) return;
        else {
            bpNode faNode;
            if (curNode.parent == root.loc) faNode = root;
            else nodeDocument.read(faNode, curNode.parent);
            int posInNode = noOrderSearch(faNode.children, 0, faNode.nodeSiz + 1, curNode.loc);
            if (posInNode) {
                if (posInNode)faNode.indexes[posInNode - 1] = indexToUpdate;
                if (faNode.loc == root.loc) root = faNode;
                else nodeDocument.update(faNode, faNode.loc);
            } else indexUpdate(faNode, indexToUpdate);
        }
//        while (posInNode == 0 && faNode.loc != root.loc) {
//            curNode = faNode;
//            if (curNode.parent == root.loc) faNode = root;
//            else nodeDocument.read(faNode, curNode.parent);
//            posInNode = binarySearch(faNode.children, 0, faNode.nodeSiz + 1, curNode.loc);
//        }
//        if (curNode.loc == root.loc && posInNode == 0)return;
//        else {
//            if(posInNode)faNode.indexes[posInNode - 1] = indexToUpdate;
//            if (faNode.loc == root.loc) root = faNode;
//            else nodeDocument.update(faNode, faNode.loc);
//        }
    }

    //闭区间 [l,r],寻找大于等于的第一个
    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::binarySearch(const int *a, int l, int r,
                                                                                      const int &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (obj <= a[mid]) r = mid;
            else l = mid + 1;
        }
        return r;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::noOrderSearch(const int *a, int l, int r,
                                                                                       const int &obj) {
        for (int i = l; i < r; ++i)
            if (a[i] == obj)return i;
        return r;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::binarySearch(const indexPair *a, int l, int r,
                                                                                      const Key &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (keyCompare(a[mid].keyOf(), obj)) l = mid + 1;
            else r = mid;
        }
        return r;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::binarySearch(const bpTree::indexPair *a, int l,
                                                                                  int r,
                                                                                  const indexPair &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (obj <= a[mid]) r = mid;
            else l = mid + 1;
        }
        return r;
    }

    //左闭右开[l,r) 返回r
    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::keySearch(const indexPair *a, int l, int r,
                                                                                   const Key &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (!keyCompare(a[mid].keyOf(), obj)) r = mid;
            else l = mid + 1;
        }
        if (r && !keyCompare(a[r - 1].keyOf(), obj) && !keyCompare(obj, a[r - 1].keyOf()))return r - 1;
        else return r;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::keySearch(const indexPair *a, int l, int r,
                                                                                   const indexPair &obj) {
        while (l < r) {
            int mid = (r + l) >> 1;
            if (a[mid] <= obj) l = mid + 1;
            else r = mid;
        }
        return r;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    typename bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::iterator
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::begin() {
        array first;
        arrayDocument.read(first, head);
        return bpTree::iterator(first.next, head, 0, first.arraySiz, this);
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    typename bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::iterator
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::find(Key key) {
        bpNode curNode = root;
        while (!curNode.isLeaf) {
            int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, key);
            nodeDocument.read(curNode, curNode.children[posInNode]);
        }
        int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, key);
        array curArray;
        arrayDocument.read(curArray, curNode.children[posInNode]);
        int posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, key);
        if (posInArray == curArray.arraySiz) {
            arrayDocument.read(curArray, curArray.next);
            ++posInNode;
            posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, key);
        }
        return iterator(curArray.next, curNode.children[posInNode], posInArray, curArray.arraySiz, this);
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::valueUpdate(bpTree::iterator iter,
                                                                                      Value newValue, int time) {
        array tem;
        iter.master->arrayDocument.read(tem, iter.pos);
        storagePair pair;
        storageDocument.read(pair, tem.data[iter.num].pos);
        if(time>0) rollback.push(time, 3, tem.data[iter.num].pos, pair.value);
        pair.value = newValue;
        storageDocument.update(pair, tem.data[iter.num].pos);
    }


    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::array::array() {
        arraySiz = 0;
        next = 0;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::array::array(bpTree::indexPair obj, int _next) {
        data[0] = obj;
        arraySiz = 1;
        next = _next;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    Value bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::dirRead(int pos) {
        storagePair result;
        storageDocument.read(result, pos);
        return result.value;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    void bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::roll() {
        rollback.logFile.open(rollback.name + "Logfile.dat");
        --rollback.logSiz;
        rollback.logFile.seekp(4 + rollback.logSiz * sizeof(Log));
        Log curLog;
        rollback.logFile.read(reinterpret_cast<char *>(&curLog), sizeof(Log));
        if (curLog.op == 1) {
            storagePair curPair;
            storageDocument.read(curPair, curLog.obj);
            erase(curPair.key, curPair.value);
        } else if (curLog.op == 2) {
            storagePair curPair;
            storageDocument.read(curPair, curLog.obj);
            insert(curPair.key, curPair.value);
        } else if (curLog.op == 3) {
            rollback.modStack.open(rollback.name + "ModStack.dat");
            --rollback.modSiz;
            rollback.modStack.seekp(4 + rollback.modSiz * sizeof(Value));
            Value oldValue;
            rollback.modStack.read(reinterpret_cast<char *>(&oldValue), sizeof(Value));
            storagePair curPair;
            storageDocument.read(curPair, curLog.obj);
            curPair.value = oldValue;
            storageDocument.update(curPair, curLog.obj);
        }
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::rollbackInsert(Key key, Value value, int pos) {
        HashType valueHash = hashFunc(value);
        indexPair obj(key, valueHash, 0);
        if (siz == 0) {
            storagePair pair(key, value);
            int valuePos = storageDocument.write(pair);
            obj.pos = valuePos;
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
        if (curArray.data[posInArray] == obj && posInArray != curArray.arraySiz) return 0;
        obj.pos = pos;
        if (posInArray == 0 && posInNode != 0) curNode.indexes[posInNode - 1] = obj;
        else if (posInNode == 0 && posInArray == 0 && curNode.loc != root.loc) indexUpdate(curNode, obj);
        for (int i = curArray.arraySiz; i > posInArray; --i) curArray.data[i] = curArray.data[i - 1];
        curArray.data[posInArray] = obj;
        ++curArray.arraySiz;
        arrayDocument.update(curArray, curNode.children[posInNode]);
        if (curNode.loc != root.loc) nodeDocument.update(curNode, curNode.loc);
        else root = curNode;
        ++siz;
        if (curArray.arraySiz == MAX_DATA) arraySplit(curNode, curArray, posInNode);
        return pos;
    }

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    int bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::rollbackErase(Key key, Value value) {
        HashType valueHash = hashFunc(value);
        indexPair obj(key, valueHash, 0);
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

    template<class Key, class Value, class HashType, class HashFunc, class KeyCompare, class HashCompare>
    typename bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::iterator
    bpTree<Key, Value, HashType, HashFunc, KeyCompare, HashCompare>::pairFind(Key key, Value value) {
        HashType valueHash = hashFunc(value);
        indexPair obj(key, valueHash, 0);
        bpNode curNode = root;
        while (!curNode.isLeaf) {
            int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, obj);
            nodeDocument.read(curNode, curNode.children[posInNode]);
        }
        int posInNode = keySearch(curNode.indexes, 0, curNode.nodeSiz, obj);
        array curArray;
        arrayDocument.read(curArray, curNode.children[posInNode]);
        int posInArray = binarySearch(curArray.data, 0, curArray.arraySiz, obj);
        iterator iter(curArray.next,curNode.children[posInNode],posInArray,curArray.arraySiz,this);
        return iter;
    }
}


#endif //TICKET_SYSTEM_DATABASE_H
