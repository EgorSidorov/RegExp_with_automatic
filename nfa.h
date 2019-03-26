#ifndef NFA_H
#define NFA_H

#include <QList>
#include <QMultiMap>
#include <QTableWidget>


class NFA
{
public:
    NFA(QList<QChar>);
    void init();
    void concat(QChar symbol);
    void association(QChar symbol);
    void kleene(QChar symbol);
    void print();
    void print(QTableWidget *);

    int get_count_nodes(){
        return nodes.count();
    }
private:
    struct node{
    public:
        //
        void add_input_node(node* value, QChar symbol, bool recursive = true){
            if(recursive){
                value->add_output_node(this,symbol,false);
            }
            input_map.insert(symbol,value);
        }

        void add_output_node(node* value, QChar symbol, bool recursive = true){
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
    QList<QList<QChar> > NFA_table;
    QList<QChar> dictionary;
    QList<node*> nodes;
    node* input_node;
    node* output_node;
};

#endif // NFA_H
