#include "Database.h"
#include "MyString.h"


int main() {
    int m;
    cin >> m;
    sjtu::bpTree<MyString, int> dataBase;
    std::string command;
    for (int i = 0; i < m; ++i) {
        cin >> command;
        if (command == "insert") {
            char key[65];
            int value;
            cin >> key >> value;
            dataBase.insert({key, value});
        } else if (command == "delete") {
            char key[65];
            int value;
            cin >> key >> value;
            dataBase.erase({key, value});
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


