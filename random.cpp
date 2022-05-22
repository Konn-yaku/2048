#include "random.h"

bool Random::isInited = false;

void Random::init() {
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    Random::isInited = true;
}

int Random::randint() {
    if (false == Random::isInited) {
        Random::init();
    }
    return qrand();
}

int Random::randint(int start, int end) {
    if (false == Random::isInited) {
        Random::init();
    }
    return (qrand() % (end - start)) + start;
}

QPair<int, int> Random::randpair()
{
    if (Random::isInited == false)
    {
        Random::init();
    }
    // ����pair
    return qMakePair(
        Random::randint(0, 4),
        Random::randint(0, 4)
    );
}
