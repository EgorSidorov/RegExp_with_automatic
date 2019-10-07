#include "tests.h"
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

    make("a|bc|de|be","a",true);
    make("a|bc|de|be","bc",true);
    make("a|bc|de|be","de",true);
    make("a|bc|de|be","be",true);
    make("a|bc|de|be","dc",false);

    make("a*b*bcd(k*|l(m*))","aaaabbbbcdkkkkkkk",true);
    make("a*b*bcd(k*|l(m*))","aaaabbbbcdkkkkkkkl",false);
    make("a*b*bcd(k*|l(m*))","aaaabbbbcdlmmmm",true);
    make("a*b*bcd(k*|l(m*))|d+","d",true);
    make("a*b*bcd(k*|l(m*))|d+","dddddddd",true);
    make("a*b*bcd(k*|l(m*))|d","aaaabbbbcdlmmmm",true);

    make("a|d+","dd",true);
    make("a|d+","a",true);
    make("a|d+","ad",false);

    make("ab*|cd+","abbbbbbbbb",true);
    make("ab*|cd+","abbbbbbbbbcd",false);

    make("(ab)*|cd+","ababababab",true);
    make("(ab)*|cd+","abababababcd",false);

    make("(a|b)*abb","ab",false);
    make("(a|b)+abb","abb",false);
}

void tests::make(QString expression,QString match, bool result){
    RegExp exp(expression);
    if(exp.Check(match) != result){
        qDebug()<<expression<<" "<<match;
        print(0,exp);
        print(1,exp);
        assert(exp.Check(match) == result);
    }
}

void tests::print(int type, RegExp & exp)
{
    int number = 1;
    QList<node*> nodes;
    if(type == 0){
        qDebug()<<"\nNFA\n";
        nodes = exp.getListNFANodes();
    }
    else if(type == 1){
        qDebug()<<"\nDFA\n";
        nodes = exp.getListDFANodes();
    }
    for(node* iter1:nodes)
    {
        QString str_node = "node";
        if(iter1->get_is_starting())
            str_node.prepend("Begin ");
        if(iter1->get_is_Ended())
            str_node.prepend("End ");
        qDebug()<<str_node+QString::number(number);
        QList<QChar> list_iter2;
        foreach(QChar iter2 , iter1->getOutgoing_map()->keys())
        {
            if(list_iter2.indexOf(iter2) != -1)
                continue;
            list_iter2<<iter2;
            int number_iter = 1;
            for(node* iter_node: nodes){
                if(iter1->getOutgoing_map()->find(iter2,iter_node) != iter1->getOutgoing_map()->end()){
                   qDebug()<<QString::number(number_iter)<<" "<<iter2;
                }
                number_iter++;

            }
        }
        number++;
    }
}
