#ifndef NFA_H
#define NFA_H

#include <QList>
#include <QMultiMap>
#include <QTableWidget>
#include <QStack>

class RegExp
{
public:
    RegExp(QList<QChar>);
    RegExp();

    void init();
    void concagenation();
    void association();
    void kleene();
    void print();
    void print(QTableWidget *);

    int get_count_nodes(){
        return nodes.count();
    }
public:

    struct node{
    public:
        //
        void add_input_node(node* value, QChar symbol, bool recursive = true){
            if(get_input_nodes(symbol).indexOf(value) != -1)
                return;
            if(symbol == '$' && this == value)
                return;
            if(recursive){
                value->add_output_node(this,symbol,false);
            }
            input_map.insert(symbol,value);
        }

        void add_output_node(node* value, QChar symbol, bool recursive = true){
            if(get_output_nodes(symbol).indexOf(value) != -1)
                return;
            if(symbol == '$' && this == value)
                return;
            if(recursive){
                value->add_input_node(this,symbol,false);
            }
            outgoing_map.insert(symbol,value);
        }
        //

        void remove_input_node(QChar symbol, bool recursive = true){
            if(recursive){
                for(node* node:get_input_nodes(symbol)){
                    node->remove_output_node(symbol,false);
                }
            }
            input_map.remove(symbol);
        }

        //
        QChar remove_input_node(node* value, bool recursive = true){
            QChar return_value;
            if(recursive){
                value->remove_output_node(this,false);
            }
            foreach(auto& iter,input_map.keys())
            {
                QList<node*> res_list = input_map.values();
                for(node* it : res_list)
                {
                    if(it == value){
                        remove_input_node(iter,value,false);
                        return_value = iter;
                    }
                }
            }
            return return_value;
        }

        void remove_input_node(QChar symbol, node* node, bool recursive = true){
            if(recursive){
                node->remove_output_node(this,false);
            }
            input_map.remove(symbol,node);
        }
        //

        //
        void remove_output_node(QChar symbol, bool recursive = true){
            if(recursive){
                for(node* node:get_output_nodes(symbol)){
                    node->remove_input_node(symbol,false);
                }
            }
            outgoing_map.remove(symbol);
        }

        QChar remove_output_node(node* value, bool recursive = true){
            QChar return_value;
            if(recursive){
                value->remove_input_node(this,false);
            }
            foreach(auto& iter,outgoing_map.keys())
            {
                QList<node*> res_list = outgoing_map.values();
                for(node* it : res_list)
                {
                    if(it == value){
                        remove_output_node(iter,value,false);
                        return_value = iter;
                    }
                }
            }
            return return_value;
        }

        void remove_output_node(QChar symbol, node* node, bool recursive = true){
            if(recursive){
                node->remove_input_node(this,false);
            }
            outgoing_map.remove(symbol,node);
        }
        //

        //
        QList<node*> get_input_nodes(QChar symbol)
        {
            QList<node*> return_list;
            QMultiMap<QChar,node*>::iterator i = input_map.find(symbol);
            while (i != input_map.end() && i.key() == symbol) {
                return_list.append(i.value());
                ++i;
            }
            return return_list;
        }

        QList<node*> get_input_nodes()
        {
            QList<node*> return_list;
            QMultiMap<QChar,node*>::iterator i = input_map.begin();
            while (i != input_map.end()) {
                return_list.append(i.value());
                ++i;
            }
            return return_list;
        }
        //

        //
        QList<QChar> get_input_nodes_keys()
        {
            return input_map.keys();
        }
        //

        //
        QList<QChar> get_output_nodes_keys()
        {
            return outgoing_map.keys();
        }
        //

        //
        QList<node*> get_output_nodes(QChar symbol)
        {
            QList<node*> return_list;
            QMultiMap<QChar,node*>::iterator i = outgoing_map.find(symbol);
            while (i != outgoing_map.end() && i.key() == symbol) {
                return_list.append(i.value());
                ++i;
            }
            return return_list;
        }

        QList<node*> get_output_nodes()
        {
            QList<node*> return_list;
            QMultiMap<QChar,node*>::iterator i = outgoing_map.begin();
            while (i != outgoing_map.end()) {
                return_list.append(i.value());
                ++i;
            }
            return return_list;
        }
        //

        /*int get_count_outgoing_nodes(QChar symbol = NULL)
        {
            if(symbol == NULL)
                return outgoing_map.count();
            else
                return outgoing_map.count(symbol);
        }*/

        /*
        int get_count_input_nodes(QChar symbol = NULL)
        {
            if(symbol == NULL)
                return input_map.count();
            else
                return input_map.count(symbol);
        }
         */

        void set_is_starting(bool value){
            isStarting = value;
        }

        bool get_is_starting(){
            return isStarting;
        }

        void set_is_Ended(bool value){
            isEnded = value;
        }

        bool get_is_Ended(){
            return isEnded;
        }


        QMultiMap<QChar,node*> input_map;
        QMultiMap<QChar,node*> outgoing_map;
        bool isStarting = false;
        bool isEnded = false;
    };

    struct dfa_struct{
        bool visited;
        bool isEnded;
        bool isStarted;
        bool calculated;
        QList<node*> states_nodes;

    };
    QList<QList<QChar> > NFA_table;
    QList<QChar> dictionary;
    QList<node*> nodes;
    node* input_node;
    QList<node*> output_node;
    bool start_is_ended;
    bool last_start_is_ended;
public:
    QList<QChar> get_connection_symbols(node *input_node, node *output_node);
    QList<node*> current_nodes;
    QStack<QString> input_symbols;
    QList<node*> stack_automat;
    node* stack_begin;
    QList<node*> stack_end;
    void setEnd();
    void plus();
private:
    void revert();
    RegExp& operator=(const RegExp& other);
};

#endif // NFA_H
