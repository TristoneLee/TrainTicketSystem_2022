# 存储结构说明



本存储结构所使用数据结构基于B+树，使用Key-Value对作为基础储存单位，以Key-ValueHash对作为索引结构单位。
通过两个文件的存储管理实现元素的高效单点修改。在一个文件中保存B+树的结构，在另一个节点中维护实际存储的数据内容。
并在B+树的底层的内存块中维护实际存储数据内容的索引。  

B+树使用以下模板参量 ：  
>class Key &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;索引值   
class Value   &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 实际储存的变量类型  
class HashType &emsp;&emsp;&emsp;&emsp;Value哈希值变量类型  
class HashFunc  &emsp;&emsp;&emsp;&emsp;Value哈希处理函数  
class KeyCompare &emsp;&emsp;&emsp; Key比较函数  
class HashCompare  &emsp;&emsp;Hash比较函数  

&nbsp;

## 工具类使用  


文件读写类  iofile: 作为内嵌类 ，实现内存与文件的交互，实现读取，删除，更新等操作

结构体 storage_pair 作为基本储存单元，并储存在storageDocument文件当中
```c++
template <class Key, class T>
struct storage_Pair{
    const Key key;
    T value;
}
```
结构体 index_pair 作为基本的索引单元，在实际元素的查找当中使用
```c++
template<class Key,class HashType>{
   const Key key
   HashType valuehash;
   int pos  　　　在内存当中的地址
}
```

类iterator实现迭代器的相关功能

类rollback维护一个操作日志目录，并在需要时可以将数据库状态回滚到目标时间

bpTree类  核心数据结构，实现数据的快速查找，插入，删除和顺序读取
  
&nbsp;  
&nbsp;  



## 对外接口：

1. ```c++
   bpTree bpTree(std::string name);
   ```

   默认构造函数，构造一个空的B+树

2. ```c++
   ~bpTree();
   ```

   默认析构函数

3. ```c++
   vector<T> query(Key key);
   ```
   查找操作，返回所有以key为索引的value值的vector实现遍历

4. ```c++
   int insert(Key key, Value value, int time = -1);
   ```
   插入操作 返回插入元素的stora_pair在对应文件的索引值，如果插入失败返回0  
   若已有目标元素则失败

5. ```c++
   bool erase(Key key, Value value, int time = -1);
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
8. ```c++
   void valueUpdate(iterator iter, Value newValue, int time = -1);
   ```  
   对对应迭代器当中的元素进行修改  
9. ```c++
   Value dirRead(int pos);
   ```    
   直接读取storageDocument当中索引为pos的元素value值   
10. ```c++
    iterator find(Key key);
    ```
    查找某一个key对应的第一个value的元素，返回指向它的迭代器，
11. ```c++
    iterator find(Key key, Value);
    ```
    查找某一个key-value对，返回指向它的迭代器，如果目标不存在返回end();
12. ```c++
    iterator begin();
    ```
    返回第一个元素对应的迭代器
13. ```c++
    void roll(int stTime);
    ```
    实现数据回滚  

&nbsp;



## 实现特点和优势：  
1.使用Hush值保存维护索引，既可以实现同一个key对多个value的映射，又可以尽可能避免value规模过大导致内存占用过大和外存读取时间过长的问题  

2.支持直接通过储存pair索引地址对元素进行读取，减少B+树结构的使用次数，减少文件读写所用时间
