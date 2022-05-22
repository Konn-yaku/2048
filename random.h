#ifndef RANDOM_H
#define RANDOM_H

#include <utility>
#include <ctime>
#include <cstdlib>
#include "QTime"


class Random {
public:
    static int randint();
    static int randint(int start, int end);
    static QPair<int, int> randpair();

private:
    Random();
    static void init();
    static bool isInited;
};

#endif // RANDOM_H
