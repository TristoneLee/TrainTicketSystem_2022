#ifndef TRAINTICKETSYSTEM_ROLLBACK_H
#define TRAINTICKETSYSTEM_ROLLBACK_H

#include <fstream>

using std::fstream;
using std::string;

template<class ValueType>
class rollback {
public:
    struct log{
        int obj=0;
        int op=0;
        int pos=0;
    };

private:
    enum operation{insert,erase,modify};
    fstream logFile;
    fstream modStack;
    string name;
    int logSiz=0;
    int modSiz=0;
    const int sizeOfValue= sizeof(ValueType);
    const int sizeOfLog=sizeof(log);

public:
    rollback(string s);
    ~rollback();
    void push(int op, int pos);
    void push(int op,int pos,const ValueType &value);
    void pop();
};

template<class ValueType>
rollback<ValueType>::rollback(string _name):name(_name) {
    logFile.open(name+"Logfile.dat");
    if (logFile) {
        logFile.seekg(0);
        logFile.read(reinterpret_cast<char *>(&logSiz), sizeof(int));
        logFile.close();
    } else {
        logFile.clear();
        logFile.open(name+"Logfile.dat", std::fstream::out);
        logFile.seekp(0);
        logFile.write(reinterpret_cast<char *>(&logSiz), sizeof(int));
        logFile.close();
    }
    modStack.open(name+"ModStack.dat");
    if (modStack) {
        modStack.seekg(0);
        modStack.read(reinterpret_cast<char *>(&logSiz), sizeof(int));
        modStack.close();
    } else {
        modStack.clear();
        modStack.open(name+"ModStack.dat", std::fstream::out);
        modStack.seekp(0);
        modStack.write(reinterpret_cast<char *>(&modSiz), sizeof(int));
        modStack.close();
    }
}

template<class ValueType>
rollback<ValueType>::~rollback() {
    modStack.open(name+"ModStack.dat");
    modStack.seekp(0);
    modStack.write(reinterpret_cast<char*>(&modSiz),sizeof(int));
    modStack.close();
    logFile.open(name+"Logfile.dat");
    logFile.seekp(0);
    logFile.write(reinterpret_cast<char*>(&logSiz),sizeof(int));
    logFile.close();
}

template<class ValueType>
void rollback<ValueType>::push(int op, int obj) {
    logFile.open(name+"Logfile.dat");
    logFile.seekp(4+logSiz*sizeOfLog);
    log tem{obj,op};
    logFile.write(reinterpret_cast<char*>(&tem),sizeOfLog);
    logFile.close();
    ++logSiz;
}

template<class ValueType>
void rollback<ValueType>::push(int op, int obj,const ValueType &value) {
    modStack.open(name+"ModStack.dat");
    int pos=4+modSiz*sizeOfValue;
    modStack.seekp(pos);
    modStack.write(reinterpret_cast<char*>(&value),sizeOfValue);
    modStack.close();
    ++modSiz;
    logFile.open(name+"Logfile.dat");
    logFile.seekp(4+logSiz*sizeOfLog);
    log tem{obj,op,pos};
    logFile.write(reinterpret_cast<char*>(&tem),sizeOfLog);
    logFile.close();
    ++logSiz;
}




#endif //TRAINTICKETSYSTEM_ROLLBACK_H
