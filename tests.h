#ifndef TESTS_H
#define TESTS_H

#include "QString"
#include "regexp.h"

class tests
{
public:
    tests();
private:
    void make(QString expression, QString match, bool result);
    void print(int type, RegExp &exp);
};

#endif // TESTS_H
