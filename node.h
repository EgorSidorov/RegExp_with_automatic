#ifndef NODE_H
#define NODE_H

#include <QChar>
#include <QMultiMap>

class node
{
public:
    void add_input_node(node* value, QChar symbol, bool recursive = true);

    void add_output_node(node* value, QChar symbol, bool recursive = true);

    void remove_input_node(QChar symbol, bool recursive = true);

    QChar remove_input_node(node* value, bool recursive = true);

    void remove_input_node(QChar symbol, node* node, bool recursive = true);

    void remove_output_node(QChar symbol, bool recursive = true);

    QChar remove_output_node(node* value, bool recursive = true);

    void remove_output_node(QChar symbol, node* node, bool recursive = true);

    QList<node*> get_input_nodes(QChar symbol);

    QList<node*> get_input_nodes();

    QList<QChar> get_input_nodes_keys();

    QList<QChar> get_output_nodes_keys();

    QList<node*> get_output_nodes(QChar symbol);

    QList<node*> get_output_nodes();

    void set_is_starting(bool value);

    bool get_is_starting();

    void set_is_Ended(bool value);

    bool get_is_Ended();

    QMultiMap<QChar, node *> *getOutgoing_map();

    void revert();

private:
    QMultiMap<QChar,node*> input_map;
    QMultiMap<QChar,node*> outgoing_map;
    bool isStarting = false;
    bool isEnded = false;
};

#endif // NODE_H
