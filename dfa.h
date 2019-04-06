#ifndef DFA_H
#define DFA_H

#include <nfauto.h>

class DFAuto : public NFAuto{
public:
    explicit DFAuto();
    ~DFAuto();
    struct dfa_struct{
        bool visited;
        bool isEnded;
        bool isStarted;
        bool calculated;
    };
    void MyMethod();

};

#endif // DFA_H
