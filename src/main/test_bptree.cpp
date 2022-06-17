#include "database.h"
#include "MyString.h"

const char *savePath1="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\cmake-build-debug\arrayDocument";
const char *savePath2="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\cmake-build-debug\nodeDocument";
const char *savePath3="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\cmake-build-debug\storageDocument";
const char *savePath4="E:\课程资料\大一下\数据结构\大作业\TrainTicketSystem_2022\cmake-build-debug\basicData";

class testHash {
public:
    int operator()(int x) { return x; }
};

int main() {
//    remove(savePath1);
//    remove(savePath2);
//    remove(savePath3);
//    remove(savePath4);
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
            dataBase.insert(key, value);
        } else if (command == "delete") {
            char key[65];
            int value;
            cin >> key >> value;
            dataBase.erase(key, value);
        } else if (command == "find") {
            char key[65];
            int value;
            cin >> key;
            sjtu::vector<int> ans = dataBase.query(key);
            if (!ans.empty()) {
                for (auto iter = ans.begin(); iter != ans.end(); ++iter) cout << *iter << " ";
                cout << '\n';
            } else cout << "null" << '\n';
        }
    }
}


