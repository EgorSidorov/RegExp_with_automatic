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

                //если можно создать ноду
                if(!summary_states.isEmpty())
                {
                    //если еще нет таких нод
                    if(!nodes_states.contains(summary_states)){
                        plus_for_last_add_index++;
                        node* new_node = make_node();
                        current_nodes.insert(current_nodes.length(),summary_states);
                        new_node->set_is_Ended(isEndedList(summary_states));
                        nodes_states.insert(nodes_states.length(),summary_states);
                        new_node->add_input_node(nodes[last_add_index-number],iter_symbol);
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

bool DFAuto::isEndedList(QList<node*>& list){
    for(node* iter_node : list){
        if(iter_node->get_is_Ended())
            return true;
    }
    return false;
}

node *DFAuto::get_input_node()
{
    return input_node;
}

void DFAuto::copy(DFAuto *object)
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
