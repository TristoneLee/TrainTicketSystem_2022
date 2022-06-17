#include "database.h"
#include "MyString.h"
#include <stdio.h>

const char *savePath1="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\arrayDocument";
const char *savePath2="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\nodeDocument";
const char *savePath3="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\storageDocument";
const char *savePath4="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\basicData";

class testHash {
public:
    int operator()(int x) { return x; }
};

int main() {
    remove(savePath1);
    remove(savePath2);
    remove(savePath3);
    remove(savePath4);
    int m;
    cin >> m;
    sjtu::bpTree<MyString, int,int, testHash> dataBase("");
    std::string command;
    for (int i = 0; i < m; ++i) {
        cin >> command;
        if (command == "insert") {
            char key[65];
            int value;
            cin >> key >> value;
            for(int j=0;j<15;++j) dataBase.insert(key, value+10*j);
        } else if (command == "delete") {
            char key[65];
            int value;
            cin >> key >> value;
            for(int j=0;j<15;++j) dataBase.erase(key, value+10*j);
        } else if (command == "find") {
            char key[65];
            int value;
            cin >> key;
            sjtu::vector<int> ans = dataBase.query(key);
//            auto iter=dataBase.find(key);
//            sjtu::vector<int> ans;
//            while( !iter.ifEnd()&&(*iter).keyOf()==key){
//                ans.push_back((*iter).valueOf());
//                ++iter;
//            }
            if (!ans.empty()) {
                for (auto iter = ans.begin(); iter != ans.end()&&*iter<10; ++iter) cout << *iter << " ";
                cout << '\n';
            } else cout << "null" << '\n';
        }
    }
}


