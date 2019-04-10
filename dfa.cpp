#include "dfa.h"

DFAuto::DFAuto(QList<QChar> alphabet)
{
    this->alphabet = alphabet;
}

DFAuto::~DFAuto()
{

}

void DFAuto::insert_node(QList<node *> states, bool isStarting, bool isEnded, QChar symbol, node *from_node){
    node* new_node;
    if(!check_states(states)){
        new_node = new node;
        nodes.append(new_node);
        nodes_states.insert(0,states);
    } else{
        new_node = nodes[nodes_states.indexOf(states)];
    }
    new_node->set_is_starting(isStarting);
    new_node->set_is_Ended(isEnded);
    if(!isStarting){
        for(QList<node*>& iter_list_nodes :nodes_states){
            for(node* iter_node : iter_list_nodes){
                if(iter_node == from_node){
                    if(!nodes[nodes_states.indexOf(iter_list_nodes)]->get_output_nodes_keys().contains(symbol)){
                        nodes[nodes_states.indexOf(iter_list_nodes)]->add_output_node(new_node,symbol);
                        nodes.removeAll(new_node);
                    }
                    else {
                        make1state(new_node,nodes[nodes_states.indexOf(iter_list_nodes)]->get_output_nodes(symbol).at(0));

                    }
                    break;
                }
            }
        }
    } else input_node = new_node;
}

void DFAuto::make1state(node* etalon, node* deleteting){
    if(etalon == deleteting)
        return;
    for(node* iter_node : deleteting->get_output_nodes()){
        QChar symbol = get_connection_symbols(iter_node,deleteting).at(0);
        if(!iter_node->get_output_nodes_keys().contains(symbol)){
            etalon->add_output_node(iter_node,symbol);
        }
        deleteting->remove_output_node(symbol);
    }

    for(node* iter_node : deleteting->get_input_nodes()){
        QChar symbol = get_connection_symbols(iter_node,deleteting).at(0);
        if(!iter_node->get_input_nodes_keys().contains(symbol)){
            etalon->add_input_node(iter_node,symbol);
        }
        deleteting->remove_input_node(symbol);
    }
    delete deleteting;
}

node *DFAuto::get_input_node()
{
    return input_node;
}


QList<QChar> DFAuto::get_self_connection(QList<node *>& states){
    QList<QChar> return_list;
    for(node* iter_state : states)
        for(node* iter_state2 : states)
            if(iter_state->get_output_nodes().contains(iter_state2))
            {
                for(QChar iter_symbol : get_connection_symbols(iter_state,iter_state2))
                    return_list.append(iter_symbol);
            }
    return return_list;
}

bool DFAuto::check_states(QList<node *>& states){
    bool is_equal = false;
    for(QList<node*> iter_states : nodes_states){
        bool is_equal_iter = true;
        if(iter_states.isEmpty() || states.isEmpty())
            continue;
        if(states.length() != iter_states.length())
            continue;
        for(int i = 0; i < states.length(); i++){
            if(states[i] != iter_states[i]){
                is_equal_iter = false;
                break;
            }
        }
        if(is_equal_iter){
            is_equal = true;
            break;
        }
    }
    return is_equal;
}
