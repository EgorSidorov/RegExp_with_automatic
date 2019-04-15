#include "tests.h"
#include "regexp.h"
#include "assert.h"
#include "QDebug"

tests::tests()
{
    make("ab","ab",true);
    make("ab","a",false);

    make("ab*","a",true);
    make("ab*","abbb",true);

    make("(ab)*","ababab",true);
    make("(ab)*","",true);
    make("(ab)*","abbbbb",false);

    make("(ab)+","ababab",true);
    make("(ab)+","",false);

    make("(a|bc)+","bc",true);
    make("(a|bc)*","abca",true);
    make("(a|bc)*","b",false);

    //make("a*b*bcd(k*|l(m*))","aaaabbbbcdkkkkkkk",true);
    //make("a*b*bcd(k*|l(m*))","aaaabbbbcdkkkkkkkl",false);
    //make("a*b*bcd(k*|l(m*))","aaaabbbbcdlmmmm",true);
    //make("a*b*bcd(k*|l(m*))|d+","d",true);
    //make("a*b*bcd(k*|l(m*))|d+","dddddddd",true);
    //make("a*b*bcd(k*|l(m*))|d","aaaabbbbcdlmmmm",true);

    make("a|d+","dd",true);
    make("a|d+","a",true);
    make("a|d+","ad",false);
}

void tests::make(QString expression,QString match, bool result){
    RegExp exp(expression);
    qDebug()<<expression<<" "<<match;
    assert(exp.Check(match) == result);
}
