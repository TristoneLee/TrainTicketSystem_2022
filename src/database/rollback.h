#ifndef TRAINTICKETSYSTEM_ROLLBACK_H
#define TRAINTICKETSYSTEM_ROLLBACK_H
#include<fstream>

using std::fstream;
using std::string;

struct Log {
    //时间戳
    int time = 0;
    //内存池内的位置
    int obj = 0;
    //操作类型 1 insert 2 delete 3 modify
    int op = 0;
    //如果操作是modify，原本的value值
    int pos = 0;
};

template<class Value>
class Rollback {
    friend class bpTree;

private:
    enum operation {
        insert, erase, modify
    };


public:
    fstream logFile;
    fstream modStack;
    string name;
    int logSiz = 0;
    int modSiz = 0;
    Rollback(string s) : name(s) {
        logFile.open(name + "Logfile.dat");
        if (logFile) {
            logFile.seekg(0);
            logFile.read(reinterpret_cast<char *>(&logSiz), sizeof(int));
            logFile.close();
        } else {
            logFile.clear();
            logFile.open(name + "Logfile.dat", std::fstream::out);
            logFile.seekp(0);
            logFile.write(reinterpret_cast<char *>(&logSiz), sizeof(int));
            logFile.close();
        }
        modStack.open(name + "ModStack.dat");
        if (modStack) {
            modStack.seekg(0);
            modStack.read(reinterpret_cast<char *>(&modSiz), sizeof(int));
            modStack.close();
        } else {
            modStack.clear();
            modStack.open(name + "ModStack.dat", std::fstream::out);
            modStack.seekp(0);
            modStack.write(reinterpret_cast<char *>(&modSiz), sizeof(int));
            modStack.close();
        }
    }

    ~Rollback() {
        modStack.open(name + "ModStack.dat");
        modStack.seekp(0);
        modStack.write(reinterpret_cast<char *>(&modSiz), sizeof(int));
        modStack.close();
        logFile.open(name + "Logfile.dat");
        logFile.seekp(0);
        logFile.write(reinterpret_cast<char *>(&logSiz), sizeof(int));
        logFile.close();
    }

    void push(int time, int op, int obj) {
        logFile.open(name + "Logfile.dat");
        logFile.seekp(4 + logSiz * sizeof(Log));
        Log tem{time, obj, op};
        logFile.write(reinterpret_cast<char *>(&tem), sizeof(Log));
        logFile.close();
        ++logSiz;
    }

    void push(int time, int op, int obj, const Value &value) {
        modStack.open(name + "ModStack.dat");
        int pos = 4 + modSiz * sizeof(value);
        modStack.seekp(pos);
        modStack.write(reinterpret_cast<const char *>(&value), sizeof(value));
        modStack.close();
        ++modSiz;
        logFile.open(name + "Logfile.dat");
        logFile.seekp(4 + logSiz * sizeof(Log));
        Log tem{time, obj, op, pos};
        logFile.write(reinterpret_cast<const char *>(&tem), sizeof(Log));
        logFile.close();
        ++logSiz;
    }
};
#endif //TRAINTICKETSYSTEM_ROLLBACK_H
