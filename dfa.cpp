#include "dfa.h"
#include <QDebug>

DFAuto::DFAuto(QList<QChar> alphabet, QList<node*> input_nodes)
{
    this->alphabet = alphabet;
    this->input_nodes = input_nodes;
    isEnded = true;
    make_dfa();
}

DFAuto::~DFAuto()
{

}

void DFAuto::make_dfa(){
    input_node = make_node();
    input_node->set_is_Ended(isEndedList(input_nodes));
    input_node->set_is_starting(true);
    int last_add_index = 0;
    QList<QList<node*>> current_nodes;
    nodes_states.insert(nodes_states.length(),input_nodes);
    current_nodes.insert(current_nodes.length(),input_nodes);
    while(!current_nodes.isEmpty()){
        qDebug()<<"last_add_index:"<<last_add_index;
        int plus_for_last_add_index = 0;
        //сохраняем текущее состояние списка элементов, по которым надо пройти
        QList<QList<node*>> current_nodes_buffer = current_nodes;
        current_nodes.clear();
        //по всем буквам алфавита
        int number = current_nodes_buffer.length();
        for(QList<node*> iter_list : current_nodes_buffer){
            number--;
        for(QChar iter_symbol : alphabet){
            QList<node*> summary_states;
            //по всем выбранным элементам найти те, которые образуют новые ноды

                for(node* iter_node : iter_list){
                    QList<node*> output_nodes = iter_node->get_output_nodes(iter_symbol);
                    add_unique_nodes(summary_states,output_nodes);
                }

            qDebug()<<summary_states.length();

            //если можно создать ноду
            if(!summary_states.isEmpty())
            {
                //если еще нет таких нод
                if(!nodes_states.contains(summary_states)){
                    plus_for_last_add_index++;
                    node* new_node = make_node();
                    current_nodes.insert(current_nodes.length(),summary_states);
                    new_node->set_is_Ended(isEndedList(summary_states));
                    //new_node->set_is_starting(isStartingList(summary_states));
                    nodes_states.insert(nodes_states.length(),summary_states);
                    new_node->add_input_node(nodes[last_add_index],iter_symbol);
                }
                //если есть, установить связь с текущей
                else {
                    int index = nodes_states.indexOf(summary_states);
                    nodes[index]->add_input_node(nodes[last_add_index-number],iter_symbol);
                    if(!nodes[index]->get_is_Ended())
                        nodes[index]->set_is_Ended(isEndedList(summary_states));
                }
            }
        }
        }
        last_add_index+= plus_for_last_add_index;
    }
    nodes_states.clear();
    current_nodes.clear();
    input_nodes.clear();
    input_nodes.append(input_node);
    output_node.clear();
    for(node* iter_node : nodes){
        if(iter_node->get_is_Ended())
            output_node.append(iter_node);
    }
}

bool DFAuto::remove_double(QList<node *> &elements){
    int position = 0;
    for(node* iter_node : elements){
        int number = 0;
        for(node* iter_node2 : elements){
            if(iter_node == iter_node2 && number!=position){
                elements.removeAt(number);
                return true;
            }
            number++;
        }
        position++;
    }
    return false;
}

void DFAuto::add_unique_nodes(QList<node*>& list, QList<node *> &elements){
    while(remove_double(elements)){}
    for(node* element: elements)
        if(!list.contains(element))
            list.append(element);
}

bool DFAuto::isStartingList(QList<node*>& list){
    for(node* iter_node : list){
        if(iter_node->get_is_starting())
            return true;
    }
    return false;
}

bool DFAuto::isEndedList(QList<node*>& list){
    for(node* iter_node : list){
        if(iter_node->get_is_Ended())
            return true;
    }
    return false;
}

void DFAuto::insert_node(QList<node *> states, bool isStarting, bool isEnded, QChar symbol, node *from_node){
    node* new_node;
    if(!check_states(states)){
        new_node = make_node();
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

void DFAuto::operator=(DFAuto *object)
{
    for(node* iter_node : nodes){
        node* new_node = object->make_node();
        new_node->set_is_starting(iter_node->get_is_starting());
        new_node->set_is_Ended(iter_node->get_is_Ended());
    }
    for(node* iter_node : nodes){
        int add_from_node_number = nodes.indexOf(iter_node);
        node* add_from_node = object->getNodes().at(add_from_node_number);
        int number = 0;
        for(node* iter_node2 : iter_node->get_output_nodes()){
            int add_to_node_number = nodes.indexOf(iter_node2);
            QChar symbol = iter_node->get_output_nodes_keys().at(number);
            node* add_to_node = object->getNodes().at(add_to_node_number);
            add_from_node->add_output_node(add_to_node,symbol);
            number++;
        }
        number = 0;
        for(node* iter_node2 : iter_node->get_input_nodes()){
            int add_to_node_number = nodes.indexOf(iter_node2);
            QChar symbol = iter_node->get_input_nodes_keys().at(number);
            node* add_to_node = object->getNodes().at(add_to_node_number);
            add_from_node->add_input_node(add_to_node,symbol);
            number++;
        }
    }
    object->isEnded = isEnded;
    object->alphabet = alphabet;
    for(node* iter_node : input_nodes){
        object->input_nodes.append(object->getNodes().at(nodes.indexOf(iter_node)));
    }
    object->input_node = object->getNodes().at(nodes.indexOf(input_node));
    for(node* iter_node : output_node){
        object->output_node.append(object->getNodes().at(nodes.indexOf(iter_node)));
    }
    object->start_is_ended = start_is_ended;
    object->last_start_is_ended = last_start_is_ended;
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
