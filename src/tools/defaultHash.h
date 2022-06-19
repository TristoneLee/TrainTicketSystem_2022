#ifndef TRAINTICKETSYSTEM_DEFAULTHASH_H
#define TRAINTICKETSYSTEM_DEFAULTHASH_H

template<class valueType>
class defaultHash {
public:
    int operator()(valueType x){return 0;}
};


#endif //TRAINTICKETSYSTEM_DEFAULTHASH_H
