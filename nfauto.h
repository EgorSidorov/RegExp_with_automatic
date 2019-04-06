#ifndef NFAuto_H
#define NFAuto_H

#include <QStack>
#include <node.h>

class NFAuto{
public:
    explicit NFAuto();
    explicit NFAuto(QList<QChar> alphabet, bool isEnded);
    ~NFAuto();

    void concagenation();
    void setEnd();
    void association();
    void kleene();
    void plus();
    void revert();

    void setStack_automat(const QList<node *> value);

    void setStack_begin(node *value);

    void addStack_end(node* value);

    void addInput_symbols(QString value);

    QList<node *> getNodes();

private:
    QList<QChar> alphabet;
    QList<node*> nodes;
    node* input_node;
    QList<node*> output_node;
    bool start_is_ended;
    bool last_start_is_ended;
    QList<node*> current_nodes;
    QStack<QString> input_symbols;
    QList<node*> stack_automat;
    node* stack_begin;
    QList<node*> stack_end;
    bool isEnded;

    void init();
    QList<QChar> get_connection_symbols(node *input_node, node *output_node);
};

#endif // NFAuto_H
