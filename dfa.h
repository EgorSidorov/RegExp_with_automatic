#ifndef DFA_H
#define DFA_H

#include <nfauto.h>
#include <QMap>
#include <QList>

class DFAuto : public NFAuto{
public:
    explicit DFAuto(QList<QChar> alphabet, QList<node *> input_nodes);
    DFAuto(){};
    ~DFAuto();
    node* get_input_node();
    void copy(DFAuto* object);
private:
    QList<QList<node*> > nodes_states;
    void make_dfa();
    void add_unique_nodes(QList<node *> &list, QList<node *> &elements);
    bool isEndedList(QList<node *> &list);
    bool remove_double(QList<node *> &elements);
};

#endif // DFA_H
