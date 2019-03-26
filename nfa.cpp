#include "nfa.h"
#include <QDebug>
#include <QTableWidgetItem>

NFA::NFA(QList<QChar> dictionary)
{
    this->dictionary = dictionary;
    this->dictionary.prepend('$');
    init();
}

void NFA::init()
{
    input_node = new node;
    input_node->set_is_starting(true);
    output_node = new node;
    output_node->set_is_Ended(true);
    nodes.append(input_node);
    nodes.append(output_node);
    nodes[0]->add_output_node(output_node,'$');
}

void NFA::concat(QChar symbol)
{
    node* new_node = new node;
    nodes.append(new_node);
    new_node->set_is_Ended(true);
    output_node->set_is_Ended(false);
    output_node->add_output_node(new_node,symbol);
    output_node = new_node;
}

void NFA::association(QChar symbol)
{
    {
        node* new_node = new node;
        nodes.append(new_node);
        new_node->set_is_starting(true);
        new_node->add_output_node(input_node,'$');
        input_node->set_is_starting(false);
        input_node = new_node;
    }

    {
        node* new_node = new node;
        nodes.append(new_node);
        input_node->add_output_node(new_node,'$');

        node* new_node2 = new node;
        nodes.append(new_node2);
        new_node->add_output_node(new_node2,symbol);

        node* new_node3 = new node;
        nodes.append(new_node3);
        output_node->add_output_node(new_node3,'$');
        output_node->set_is_Ended(false);
        new_node3->set_is_Ended(true);
        output_node = new_node3;
        new_node2->add_output_node(new_node3,'$');
    }
}

void NFA::kleene(QChar symbol)
{
    concat('$');

    {
        node* new_node = new node;
        nodes.append(new_node);
        new_node->set_is_Ended(true);
        output_node->set_is_Ended(false);
        output_node->add_output_node(new_node,symbol);
        output_node->add_input_node(new_node,'$');
        output_node = new_node;
    }

    concat('$');
}

void NFA::print()
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

void NFA::print(QTableWidget *widget)
{
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
        if(iter1->isStarting){
            color = 1;
            for(int j = 0; j < widget->columnCount(); j++)
            {
                widget->item(number,j)->setBackgroundColor("yellow");
            }
        }
        if(iter1->isEnded){
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
                    if(item == NULL)
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
                    widget->setItem(number,dictionary.indexOf(iter2),new_item);
                }
                number_iter++;

            }
        }
        number++;
    }
}
