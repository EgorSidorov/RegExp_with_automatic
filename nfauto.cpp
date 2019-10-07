#include "nfauto.h"

NFAuto::NFAuto()
{

}

NFAuto::NFAuto(QList<QChar> alphabet, bool isEnded)
{
    this->isEnded = isEnded;
    this->alphabet = alphabet;
    init();
}

NFAuto::~NFAuto()
{
    if(isEnded)
        delete_nodes();
}

void NFAuto::init()
{
    input_node = make_node();
    input_node->set_is_starting(true);
    current_nodes.append(input_node);
}

void NFAuto::concagenation()
{
    if(input_symbols.isEmpty())
        return;
    QString symbols = input_symbols.pop();
    if(symbols.length() == 1){
        QChar symbol = symbols[0];
        node* new_node = make_node();
        for(node* _node : current_nodes){
            _node->add_output_node(new_node,symbol);
        }
        current_nodes.clear();
        current_nodes.append(new_node);
    } else {
        //stack_begin->set_is_starting(false);
        for(node* _node : current_nodes){
            for(node* _node2 : stack_begin->get_output_nodes()){
                    for(QChar symbol2 : get_connection_symbols(stack_begin,_node2))
                        _node->add_output_node(_node2,symbol2);
                    //stack_begin->remove_output_node(_node2);
            }
            for(node* _node2 : stack_begin->get_output_nodes()){
                stack_begin->remove_output_node(_node2);
            }
            for(node* _node2 : stack_begin->get_input_nodes()){
                    for(QChar symbol2 : get_connection_symbols(_node2,stack_begin))
                        _node->add_input_node(_node2,symbol2);
                    //stack_begin->remove_input_node(_node2);
            }
            for(node* _node2 : stack_begin->get_input_nodes()){
                stack_begin->remove_input_node(_node2);
            }
        }
        //if(!stack_end.contains(stack_begin))
            current_nodes.clear();
        //if(!stack_end.contains(stack_begin))
            stack_automat.removeAll(stack_begin);
        nodes.append(stack_automat);
        current_nodes.append(stack_end);
        for(node* _node : stack_end){
            _node->set_is_Ended(false);
        }
        stack_automat.clear();
        //if(!stack_end.contains(stack_begin))
            delete stack_begin;
        stack_begin = NULL;
        stack_end.clear();
    }
}

void NFAuto::setEnd(){
    for(node* _node : current_nodes){
        _node->set_is_Ended(true);
        output_node.append(_node);
    }
}

void NFAuto::association()
{
    output_node.append(current_nodes);
    for(node* _node : current_nodes)
        _node->set_is_Ended(true);
    current_nodes.clear();
    current_nodes.append(input_node);
}


void NFAuto::plus()
{
    if(input_symbols.isEmpty())
        return;
    QString symbols = input_symbols.pop();
    if(symbols.length() == 1){
        QChar symbol = symbols[0];
        node* new_node = make_node();
        for(node* _node : current_nodes){
            _node->add_output_node(new_node,symbol);
        }
        new_node->add_output_node(new_node,symbol);
        current_nodes.clear();
        current_nodes.append(new_node);
    }
}

void NFAuto::kleene()
{
    if(input_symbols.isEmpty())
        return;
    QString symbols = input_symbols.pop();
    if(symbols.length() == 1){
        QChar symbol = symbols[0];
        node* new_node = make_node();
        for(node* _node : current_nodes){
            _node->add_output_node(new_node,symbol);
        }
        new_node->add_output_node(new_node,symbol);
        current_nodes.append(new_node);
    } else {
        //добавить в список нод все, кроме начальных элементов(т.к. они будут удалены)
        if(!stack_end.contains(stack_begin))
            stack_automat.removeAll(stack_begin);
        nodes.append(stack_automat);

        for(node* iter_node : stack_end){
           iter_node->set_is_Ended(false);
        }

        //убрать все конечные элементы и все пути с них перенаправить на начало
        for(node* _node2 : stack_begin->get_output_nodes()){
            for(QChar symbol : get_connection_symbols(stack_begin,_node2)){
                for(node* _node3 : stack_end){
                    _node3->add_output_node(_node2,symbol);
                }
                //из текущих конечных путей поставить связи
                for(node* _node4 : current_nodes){
                    _node4->add_output_node(_node2,symbol);
                }
            }
            //stack_begin->remove_output_node(_node2);
        }

        for(node* _node2 : stack_begin->get_output_nodes()){
            stack_begin->remove_output_node(_node2);
        }

        current_nodes.append(stack_end);

        stack_automat.clear();
        if(!stack_end.contains(stack_begin))
            delete stack_begin;
        stack_begin = NULL;
        stack_end.clear();
    }
}

void NFAuto::revert()
{
    for(auto _node : nodes)
        _node->revert();
    QList<node*> buffer = output_node;
    output_node = input_nodes;
    input_nodes = buffer;
}

node* NFAuto::make_node()
{
    node* new_node = new node;
    nodes.append(new_node);
    return new_node;
}

void NFAuto::delete_nodes()
{
    for(node* iter_node: nodes)
        delete iter_node;
    nodes.clear();
}

QList<QChar> NFAuto::get_connection_symbols(node* input_node, node* output_node){
    QList<QChar> return_list;
    for(QChar symbol : alphabet){
        if(input_node->get_output_nodes(symbol).indexOf(output_node) != -1)
            return_list.append(symbol);
    }
    return return_list;
}

void NFAuto::setStack_automat(const QList<node *> value)
{
    stack_automat = value;
}

void NFAuto::setStack_begin(node *value)
{
    stack_begin = value;
}

void NFAuto::addStack_end(node *value)
{
    stack_end.append(value);
}

void NFAuto::addInput_symbols(QString value)
{
   input_symbols.push(value);
}

QList<node *> NFAuto::getNodes()
{
    return nodes;
}

void NFAuto::set_Input_Output()
{
    input_nodes.clear();
    output_node.clear();
    for(node* _node : nodes){
        if(_node->get_is_Ended())
            output_node.append(_node);
        if(_node->get_is_starting())
            input_nodes.append(_node);
    }
}

QList<node *> NFAuto::getInput_nodes() const
{
    return input_nodes;
}
