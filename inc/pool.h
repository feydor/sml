#ifndef POOL_H
#define POOL_H

template <T>
class Pool {
    public:
        T request();

    private:
        static const int POOL_SIZE = 100;
        T objects[POOL_SIZE];
};

#endif

