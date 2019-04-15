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
    void insert_node(QList<node*> states, bool isStarting, bool isEnded, QChar symbol, node* from_node);
    node* get_input_node();
    void operator=(DFAuto* object);
private:
    QList<QList<node*> > nodes_states;
    bool check_states(QList<node *> &states);
    QList<QChar> get_self_connection(QList<node *>& states);
    void make1state(node *etalon, node *deleteting);
    void make_dfa();
    void add_unique_nodes(QList<node *> &list, QList<node *> &elements);
    bool isStartingList(QList<node *> &list);
    bool isEndedList(QList<node *> &list);
    bool remove_double(QList<node *> &elements);
};

#endif // DFA_H
