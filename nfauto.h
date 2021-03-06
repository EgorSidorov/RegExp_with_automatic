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

    void set_Input_Output();

    QList<node *> getInput_nodes() const;
    void delete_nodes();
protected:
    QList<QChar> alphabet;
    QList<node*> nodes;
    QList<node*> input_nodes;
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
    node* make_node();
    void init();
    QList<QChar> get_connection_symbols(node *input_node, node *output_node);
};

#endif // NFAuto_H
