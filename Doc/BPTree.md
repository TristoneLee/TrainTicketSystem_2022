# 存储结构说明

-----

本存储结构所使用数据结构基于B+树，使用Key-Value对作为基础储存单位，并使用调用vector库



文件中包含以下组成：

文件读写类  iofile: 作为内嵌类 ，实现内存与文件的交互，实现读取，删除，更新等操作

结构体 pair 进行基本储存单元的管理

```c++
template <class Key, class T>
struct pair{
    const Key key;
    T value;
}
```

bpTree类  核心数据结构，实现数据的快速查找，插入，删除和顺序读取



对外接口：

1. ```c++
   bpTree();
   ```

   默认构造函数，构造一个空的B+树

2. ```c++
   ~bpTree();
   ```

   默认析构函数

3. ```c++
   vector<T> query(Key key);
   ```

   查找操作，返回一个所有以key为索引的value值

4. ```c++
   bool insert (pair obj);
   ```

   插入操作 返回是否插入成功，成功返回1，失败返回0

   若已有目标元素则失败

5. ```c++
   bool erase(pair obj);
   ```

   删除操作 返回是否删除成功，成功返回1，失败返回0

   若无目标元素则失败

6. ```c++
   size_t size();
   ```

   返回B+树中元素数量

7. ```c++
   void clear();
   ```

   清空B+树中元素

