#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dictionary << '*' << '|';
    nfa = nullptr;
}

MainWindow::~MainWindow()
{
    if(nfa != nullptr)
        delete nfa;
    delete ui;
}

void MainWindow::on_checkButton_clicked()
{
    QString result;
    if(check_valid_string(ui->regExpEdit->text(),result)){
        write_dictionary(ui->regExpEdit->text(), non_terminal);
    }
    ui->result_label->setText(result);
}

bool MainWindow::check_valid_string(QString const& string, QString& description)
{
    //проверка равенства числа левых и правых скобок
    {
        int index = 0;
        for(QChar symbol:string){
            if(symbol == '(')
                index++;
            else if(symbol == ')')
                index--;
            if(index < 0){
                description = "Некорректное. Закрывающаяся скобка без открывающейся.";
                return false;
            }
        }
        if(index != 0){
            description = "Некорректное. Открывающаяся скобка без закрывающейся.";
            return false;
        }
    }
    //проверка равенства числа левых и правых скобок

    //проверка корректности ввода операций
    {
        int currentIndexDictionary;
        int lastIndexDictionary = NULL;
        for(QChar symbol:string){
            currentIndexDictionary = dictionary.indexOf(symbol);
            if(currentIndexDictionary != -1 && lastIndexDictionary != NULL)
            {
                if(currentIndexDictionary == lastIndexDictionary){
                    //description = "Некорректное. Два терминальных символа подряд.";
                    //return false;
                }
            }
            lastIndexDictionary = currentIndexDictionary;
        }
    }

    //проверка корректности ввода терминальных символов

    description = "Корректное";
    return true;
}

void MainWindow::write_dictionary(const QString &string, QList<QChar> &non_terminal_value)
{
    for(QChar symbol:string)
    {
        if(non_terminal_value.indexOf(symbol) == -1 && dictionary.indexOf(symbol) == -1 && symbol != '(' && symbol != ')')
            non_terminal_value.append(symbol);
    }
    create_nfa(string);
    /*int countRow = nfa->get_count_state();
    int countColumn = nfa->get_lenght_dictionary();

    ui->tableWidgetDNA->setColumnCount(countColumn);
    ui->tableWidgetDNA->setRowCount(countRow);
    for(int i = 0; i < countColumn; i++)
    {
        ui->tableWidgetDNA->setHorizontalHeaderItem(i,new QTableWidgetItem(non_terminal_value.at(i)));
        for(int j = 0; j < countRow; j++)
        {
            QTableWidgetItem* tableItem = new QTableWidgetItem(QString(nfa->get_item(j,i)));
            ui->tableWidgetDNA->setItem(i,j,tableItem);
        }
    }
    ui->tableWidgetDNA->update();*/
}

void MainWindow::choose_bracket(QString &string, QStringList &list_special_string)
{

}
void MainWindow::create_nfa(const QString &string)
{
    nfa = new NFA(non_terminal);
    int pos = 0;
    bool pass = false;
    for(QChar symbol:string)
    {
        if(pass){
            pass = false;
            pos++;
            continue;
        }
        if(symbol == '|'){
            pass = true;
            nfa->association(string[pos+1]);
        } else if(string.length()>(pos+1) && string[pos+1] == '*'){
            pass = true;
            nfa->kleene(symbol);
        } else {
            nfa->concat(symbol);
        }
        pos++;
    }
    nfa->print();
    nfa->print(ui->tableWidgetDNA);
}

