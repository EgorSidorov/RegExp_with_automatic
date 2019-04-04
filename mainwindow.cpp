#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bracketstosymbol.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dictionary << '*' << '|' << '+'<<'('<<')'<<'&';
    nfa = nullptr;
    ui->comboBox->addItem("НКА");
    ui->comboBox->addItem("ДКА");
    comboBoxIndex = 0;
}

MainWindow::~MainWindow()
{
    if(nfa != nullptr)
        delete nfa;
    delete ui;
}

void MainWindow::on_checkButton_clicked()
{
}

void MainWindow::makeNFAfromRegExp(){
    QString result;
    QString standart_str = ui->regExpEdit->text();
    if(check_valid_string(standart_str,result)){
        make_alphabet(standart_str);
        change_plus_by_kleene_and_contagenation(standart_str);
        BracketsToSymbol list_brackets_symbols(standart_str);
        for(auto& it_str : list_brackets_symbols.get_bracket_list_priority()){
            create_nfa(it_str.second,list_brackets_symbols.get_bracket_node_map());
            list_brackets_symbols.insert_bracket_node_map(it_str.first,nfa->nodes);
        }
        create_nfa(standart_str,list_brackets_symbols.get_bracket_node_map());
        if(comboBoxIndex == 1){
            dfa = std::make_unique<DFA>();
        }
        nfa->print(ui->tableWidgetDNA);
    } else{
         ui->result_label->setText(result);
    }
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

    description = "Корректное";
    return true;
}

void MainWindow::make_alphabet(QString &string)
{
    alphabet.clear();
    for(QChar symbol:string){
        if(alphabet.indexOf(symbol) == -1 && dictionary.indexOf(symbol) == -1)
            alphabet.append(symbol);
    }
}

void MainWindow::add_sign(QString& string){
    if(string[0] != '/')
        string.insert(1,'&');
    else
        string.insert(3,'&');
    int pos = 2;
    while(pos < string.length()){
        QChar symbol = string[pos];
        if(symbol == '/'){
            string.insert(pos+3,'&');
            pos=pos+3;
        }
        else if(dictionary.indexOf(symbol) == -1){
                string.insert(pos+1,'&');
                pos++;
        } else if(symbol == '*' || symbol == '+'){
            string.remove(pos-1,1);
            pos--;
        }
        pos++;
    }
    if(dictionary.indexOf(string[string.length()-1]) == -1)
        string.append('&');

}

void MainWindow::change_plus_by_kleene_and_contagenation(QString& string){
    int current_index = 0;
    int plus_index = string.indexOf(")+",current_index);
    int begin_index = 0;

    while(plus_index != -1){
        int number = 1;
        while(number != 0){
            for(int j = plus_index-1; j>=0; j--){
                if(string[j] == '(')
                    number--;
                if(string[j] == ')')
                    number++;
                if(number == 0)
                {
                    begin_index = j;
                    break;
                }
            }
        }
        QString current_string = string.mid(begin_index,plus_index-begin_index+1);
        string.remove(plus_index+1,1);
        string.insert(plus_index+1,"*"+current_string);
        plus_index+= current_string.length()+1;
        current_index = plus_index;
        plus_index = string.indexOf(")+",current_index);
    }
}

void MainWindow::create_nfa(QString &string, QMap<QString, QList<RegExp::node*> > list_brackets_nodes)
{
    add_sign(string);
    if(nfa != nullptr)
        delete nfa;
    nfa = new RegExp(alphabet);
    int pos = 0;
    while(string.length() > pos)
    {
        QChar symbol = string[pos];
        if(string.length()>=(pos+3) && (symbol == '/')){
            QString str = string.mid(pos,3);
            nfa->input_symbols.push(str);
            nfa->stack_automat = list_brackets_nodes.find(str).value();
            for(RegExp::node* _node : nfa->stack_automat){
                if(_node->get_is_starting())
                    nfa->stack_begin = _node;
                if(_node->get_is_Ended())
                    nfa->stack_end.append(_node);
            }
            pos=pos+2;
        }
        else if(symbol == '|'){
            nfa->association();
        } else if(symbol == '*'){
            nfa->kleene();
        } else if(symbol == '+'){
            nfa->plus();
        } else if(symbol == '&'){
            nfa->concagenation();
        }else{
            nfa->input_symbols.push(symbol);
        }
        pos++;
    }
    nfa->setEnd();
}


void MainWindow::on_comboBox_activated(int index)
{
    comboBoxIndex = index;
}

void MainWindow::on_genButton_clicked()
{
    makeNFAfromRegExp();
}
