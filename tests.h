#ifndef TESTS_H
#define TESTS_H

#include "QString"

class tests
{
public:
    tests();
private:
    void make(QString expression, QString match, bool result);
};

#endif // TESTS_H
