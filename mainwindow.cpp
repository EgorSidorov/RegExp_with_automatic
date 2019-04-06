#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bracketstosymbol.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("НКА");
    ui->comboBox->addItem("ДКА");
    comboBoxIndex = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkButton_clicked()
{
}


void MainWindow::on_comboBox_activated(int index)
{
    comboBoxIndex = index;
}

void MainWindow::on_genButton_clicked()
{
    regExp = std::make_unique<RegExp>(ui->regExpEdit->text());
    if(regExp->getIsValid())
        print(ui->tableWidget);
    ui->result_label->setText(regExp->getDescription());
}

void MainWindow::print()
{
    int number = 1;
    QList<node*> nodes;
    if(comboBoxIndex == 0)
        nodes = regExp->getListNFANodes();
    for(node* iter1:nodes)
    {
        QString str_node = "node";
        if(iter1->get_is_starting())
            str_node.prepend("Begin ");
        if(iter1->get_is_Ended())
            str_node.prepend("End ");
        qDebug()<<str_node+QString::number(number);
        QList<QChar> list_iter2;
        foreach(QChar iter2 , iter1->getOutgoing_map()->keys())
        {
            if(list_iter2.indexOf(iter2) != -1)
                continue;
            list_iter2<<iter2;
            int number_iter = 1;
            for(node* iter_node: nodes){
                if(iter1->getOutgoing_map()->find(iter2,iter_node) != iter1->getOutgoing_map()->end()){
                   qDebug()<<QString::number(number_iter)<<" "<<iter2;
                }
                number_iter++;

            }
        }
        number++;
    }
}

void MainWindow::print(QTableWidget *widget)
{
    widget->clear();
    QList<node*> nodes;
    if(comboBoxIndex == 0)
        nodes.append(regExp->getListNFANodes());
    int countRow = nodes.length();
    int countColumn = regExp->getAlphabet().length();
    widget->setColumnCount(countColumn);
    widget->setRowCount(countRow);
    for(int i = 0; i < countColumn; i++)
    {
       widget->setHorizontalHeaderItem(i,new QTableWidgetItem(regExp->getAlphabet().at(i)));
       for(int j = 0; j < countRow; j++)
       {
           QTableWidgetItem* tableItem = new QTableWidgetItem(QString(" "));
           widget->setItem(i,j,tableItem);
       }
    }

    for(int i = 0; i < countColumn; i++)
    {
       widget->setHorizontalHeaderItem(i,new QTableWidgetItem(regExp->getAlphabet().at(i)));
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
        if(iter1->get_is_starting() && iter1->get_is_Ended()){
            color = 3;
            for(int j = 0; j < widget->columnCount(); j++)
            {
                widget->item(number,j)->setBackgroundColor("orange");
            }
        }
        else if(iter1->get_is_starting()){
            color = 1;
            for(int j = 0; j < widget->columnCount(); j++)
            {
                widget->item(number,j)->setBackgroundColor("yellow");
            }
        }
        else if(iter1->get_is_Ended()){
            color = 2;
            for(int j = 0; j < widget->columnCount(); j++)
            {
                widget->item(number,j)->setBackgroundColor("red");
            }
        }
        QList<QChar> list_iter2;
        for(QChar iter2 : iter1->getOutgoing_map()->keys())
        {
            if(list_iter2.indexOf(iter2) != -1)
                continue;
            list_iter2<<iter2;
            int number_iter = 1;
            for(node* iter_node: nodes){
                if(iter1->getOutgoing_map()->find(iter2,iter_node) != iter1->getOutgoing_map()->end()){
                    QTableWidgetItem * item = widget->item(number,regExp->getAlphabet().indexOf(iter2));
                    if(item == nullptr)
                        continue;
                        //exit(1);
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
                    widget->setItem(number,regExp->getAlphabet().indexOf(iter2),new_item);
                }
                number_iter++;

            }
        }
        number++;
    }
}
