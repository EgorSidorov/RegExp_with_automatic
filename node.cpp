#include "node.h"

void node::add_input_node(node *value, QChar symbol, bool recursive){
    if(get_input_nodes(symbol).indexOf(value) != -1)
        return;
    if(recursive){
        value->add_output_node(this,symbol,false);
    }
    input_map.insert(symbol,value);
}

void node::add_output_node(node *value, QChar symbol, bool recursive){
    if(get_output_nodes(symbol).indexOf(value) != -1)
        return;
    if(recursive){
        value->add_input_node(this,symbol,false);
    }
    outgoing_map.insert(symbol,value);
}

void node::remove_input_node(QChar symbol, bool recursive){
    if(recursive){
        for(node* node:get_input_nodes(symbol)){
            node->remove_output_node(symbol,false);
        }
    }
    input_map.remove(symbol);
}

QChar node::remove_input_node(node *value, bool recursive){
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

void node::remove_input_node(QChar symbol, node *node, bool recursive){
    if(recursive){
        node->remove_output_node(this,false);
    }
    input_map.remove(symbol,node);
}

void node::remove_output_node(QChar symbol, bool recursive){
    if(recursive){
        for(node* node:get_output_nodes(symbol)){
            node->remove_input_node(symbol,false);
        }
    }
    outgoing_map.remove(symbol);
}

QChar node::remove_output_node(node *value, bool recursive){
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

void node::remove_output_node(QChar symbol, node *node, bool recursive){
    if(recursive){
        node->remove_input_node(this,false);
    }
    outgoing_map.remove(symbol,node);
}

QList<node *> node::get_input_nodes(QChar symbol)
{
    return input_map.values(symbol);
}

QList<node *> node::get_input_nodes()
{
    return input_map.values();
}

QList<QChar> node::get_input_nodes_keys()
{
    return input_map.keys();
}

QList<QChar> node::get_output_nodes_keys()
{
    return outgoing_map.keys();
}

QList<node *> node::get_output_nodes(QChar symbol)
{
    return  outgoing_map.values(symbol);
}

QList<node *> node::get_output_nodes()
{
    return  outgoing_map.values();
}

void node::set_is_starting(bool value){
    isStarting = value;
}

bool node::get_is_starting(){
    return isStarting;
}

void node::set_is_Ended(bool value){
    isEnded = value;
}

bool node::get_is_Ended(){
    return isEnded;
}

QMultiMap<QChar, node *>* node::getOutgoing_map()
{
    return &outgoing_map;
}

void node::revert()
{
    QMultiMap<QChar,node*> buffer = input_map;
    input_map = outgoing_map;
    outgoing_map = buffer;
    if(isStarting && isEnded)
        return;
    bool buffer_bool = isStarting;
    isStarting = isEnded;
    isEnded = buffer_bool;
}
