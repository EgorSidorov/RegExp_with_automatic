#include "regexp.h"
#include <QDebug>
#include <QTableWidgetItem>

RegExp::RegExp(QList<QChar> dictionary)
{
    this->dictionary = dictionary;
    init();
}

RegExp::RegExp()
{
}

void RegExp::init()
{
    input_node = new node;
    nodes.append(input_node);
    input_node->set_is_starting(true);
    current_nodes.append(input_node);
}

void RegExp::concagenation()
{
    QString symbols = input_symbols.pop();
    if(symbols.length() == 1){
        QChar symbol = symbols[0];
        node* new_node = new node;
        nodes.append(new_node);
        for(node* _node : current_nodes){
            _node->add_output_node(new_node,symbol);
        }
        current_nodes.clear();
        current_nodes.append(new_node);
    } else {
        stack_begin->set_is_starting(false);
        for(node* _node : current_nodes){
            for(node* _node2 : stack_begin->get_output_nodes()){
                //if(_node2 != stack_begin){
                    for(QChar symbol2 : get_connection_symbols(stack_begin,_node2))
                        _node->add_output_node(_node2,symbol2);
                    stack_begin->remove_output_node(_node2);
                //}
            }
            for(node* _node2 : stack_begin->get_input_nodes()){
                //if(_node2 != stack_begin){
                    for(QChar symbol2 : get_connection_symbols(_node2,stack_begin))
                        _node->add_input_node(_node2,symbol2);
                    stack_begin->remove_input_node(_node2);
                //}
            }
        }
        if(!stack_end.contains(stack_begin))
            current_nodes.clear();
        stack_automat.removeAll(stack_begin);
        nodes.append(stack_automat);
        current_nodes.append(stack_end);
        for(node* _node : stack_end){
            _node->set_is_Ended(false);
        }
        stack_automat.clear();
        delete stack_begin;
        stack_begin = NULL;
        stack_end.clear();
    }
}

void RegExp::setEnd(){
    for(node* _node : current_nodes){
        _node->set_is_Ended(true);
    }
}

void RegExp::association()
{
    output_node.append(current_nodes);
    for(node* _node : current_nodes)
        _node->set_is_Ended(true);
    current_nodes.clear();
    current_nodes.append(input_node);
}


void RegExp::plus()
{
    QString symbols = input_symbols.pop();
    if(symbols.length() == 1){
        QChar symbol = symbols[0];
        node* new_node = new node;
        nodes.append(new_node);
        for(node* _node : current_nodes){
            _node->add_output_node(new_node,symbol);
        }
        new_node->add_output_node(new_node,symbol);
        current_nodes.clear();
        current_nodes.append(new_node);
    }
}

void RegExp::kleene()
{
    QString symbols = input_symbols.pop();
    if(symbols.length() == 1){
        QChar symbol = symbols[0];
        node* new_node = new node;
        nodes.append(new_node);
        for(node* _node : current_nodes){
            _node->add_output_node(new_node,symbol);
        }
        new_node->add_output_node(new_node,symbol);
        current_nodes.append(new_node);
    } else {
        //добавить в список нод все, кроме начальных и конечных элементов(т.к. они будут удалены)
        if(!stack_end.contains(stack_begin))
            stack_automat.removeAll(stack_begin);
        for(node* _node : stack_end){
            stack_automat.removeAll(_node);
        }
        nodes.append(stack_automat);

        //убрать все конечные элементы и все пути с них перенаправить на начало
        for(node* _node2 : stack_end){
            if(_node2 != stack_begin){
                for(node* node3 : _node2->get_output_nodes()){
                    for(QChar symbol3 : get_connection_symbols(_node2,node3)){
                        for(node* _node4 : current_nodes){
                            node3->add_input_node(_node4,symbol3);
                        }
                        //stack_begin->add_input_node(node3,symbol3);
                        //if(!current_nodes.contains(node3))
                        //    current_nodes.append(node3);
                    }
                    _node2->remove_output_node(node3);
                }
                for(node* node3 : _node2->get_input_nodes()){
                    for(QChar symbol3 : get_connection_symbols(node3,_node2)){
                        for(node* _node4 : current_nodes){
                            node3->add_output_node(_node4,symbol3);
                        }
                        //stack_begin->add_output_node(node3,symbol3);
                        //if(!current_nodes.contains(node3))
                        //    current_nodes.append(node3);
                    }
                    _node2->remove_input_node(node3);
                }
                delete _node2;
            }
        }

        //убрать начало и все пути с него перенаправить на конечные элементы
        for(node* _node : current_nodes){
            for(node* _node2 : stack_begin->get_output_nodes()){
                for(QChar symbol2 : get_connection_symbols(stack_begin,_node2))
                    _node->add_output_node(_node2,symbol2);
                stack_begin->remove_output_node(_node2);
            }
            for(node* _node2 : stack_begin->get_input_nodes()){
                for(QChar symbol2 : get_connection_symbols(_node2,stack_begin))
                    _node->add_input_node(_node2,symbol2);
                stack_begin->remove_input_node(_node2);
            }
        }

        stack_automat.clear();
        delete stack_begin;
        stack_begin = NULL;
        stack_end.clear();
    }
}

void RegExp::revert()
{

}

QList<QChar> RegExp::get_connection_symbols(node* input_node, node* output_node){
    QList<QChar> return_list;
    for(QChar symbol : dictionary){
        if(input_node->get_output_nodes(symbol).indexOf(output_node) != -1)
            return_list.append(symbol);
    }
    return return_list;
}

void RegExp::print()
{
    int number = 1;
    for(node* iter1:nodes)
    {
        QString str_node = "node";
        if(iter1->isStarting)
            str_node.prepend("Begin ");
        if(iter1->isEnded)
            str_node.prepend("End ");
        qDebug()<<str_node+QString::number(number);
        QList<QChar> list_iter2;
        foreach(QChar iter2 , iter1->outgoing_map.keys())
        {
            if(list_iter2.indexOf(iter2) != -1)
                continue;
            list_iter2<<iter2;
            int number_iter = 1;
            for(node* iter_node: nodes){
                if(iter1->outgoing_map.find(iter2,iter_node) != iter1->outgoing_map.end()){
                   qDebug()<<QString::number(number_iter)<<" "<<iter2;
                }
                number_iter++;

            }
        }
        number++;
    }
}

void RegExp::print(QTableWidget *widget)
{
    widget->clear();
    int countRow = nodes.length();
    int countColumn = dictionary.length();
    widget->setColumnCount(countColumn);
    widget->setRowCount(countRow);
    for(int i = 0; i < countColumn; i++)
    {
       widget->setHorizontalHeaderItem(i,new QTableWidgetItem(dictionary.at(i)));
       for(int j = 0; j < countRow; j++)
       {
           QTableWidgetItem* tableItem = new QTableWidgetItem(QString(" "));
           widget->setItem(i,j,tableItem);
       }
    }

    for(int i = 0; i < countColumn; i++)
    {
       widget->setHorizontalHeaderItem(i,new QTableWidgetItem(dictionary.at(i)));
       for(int j = 0; j < countRow; j++)
       {
           QTableWidgetItem* tableItem = new QTableWidgetItem(QString());
           widget->setItem(j,i,tableItem);
       }
    }

    int number = 0;
    for(node* iter1:nodes)
    {
        int color = 0;
        if(iter1->isStarting && iter1->isEnded){
            color = 3;
            for(int j = 0; j < widget->columnCount(); j++)
            {
                widget->item(number,j)->setBackgroundColor("orange");
            }
        }
        else if(iter1->isStarting){
            color = 1;
            for(int j = 0; j < widget->columnCount(); j++)
            {
                widget->item(number,j)->setBackgroundColor("yellow");
            }
        }
        else if(iter1->isEnded){
            color = 2;
            for(int j = 0; j < widget->columnCount(); j++)
            {
                widget->item(number,j)->setBackgroundColor("red");
            }
        }
        QList<QChar> list_iter2;
        foreach(QChar iter2 , iter1->outgoing_map.keys())
        {
            if(list_iter2.indexOf(iter2) != -1)
                continue;
            list_iter2<<iter2;
            int number_iter = 1;
            for(node* iter_node: nodes){
                if(iter1->outgoing_map.find(iter2,iter_node) != iter1->outgoing_map.end()){
                    QTableWidgetItem * item = widget->item(number,dictionary.indexOf(iter2));
                    if(item == nullptr)
                        exit(1);
                    QString last_text = item->text();
                    QString current_text;
                    if(!last_text.isEmpty())
                        current_text = last_text+","+QString::number(number_iter);
                    else
                        current_text = QString::number(number_iter);
                    QTableWidgetItem * new_item = new QTableWidgetItem(current_text);
                    if(color == 1)
                        new_item->setBackgroundColor(QColor("yellow"));
                    else if(color == 2)
                        new_item->setBackgroundColor(QColor("red"));
                    else if(color == 3)
                        new_item->setBackgroundColor(QColor("orange"));
                    widget->setItem(number,dictionary.indexOf(iter2),new_item);
                }
                number_iter++;

            }
        }
        number++;
    }
}
