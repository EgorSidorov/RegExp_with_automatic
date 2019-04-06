#include "regexp.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <bracketstosymbol.h>

RegExp::RegExp(const QString &str)
{
    expression = str;
    dictionary << '*' << '|' << '+'<<'('<<')'<<'&';
    if(check_valid_string()){
        make_alphabet();
        delete_empty_brackets();
        change_plus_by_kleene_and_contagenation();
        BracketsToSymbol list_brackets_symbols(expression);
        for(auto& it_str : list_brackets_symbols.get_bracket_list_priority()){
            create_nfa(it_str.second,list_brackets_symbols.get_bracket_node_map(), false);
            list_brackets_symbols.insert_bracket_node_map(it_str.first,nfa->getNodes());
        }
        create_nfa(expression,list_brackets_symbols.get_bracket_node_map(), true);
    }
}

void RegExp::make_alphabet()
{
    alphabet.clear();
    for(QChar symbol:expression){
        if(alphabet.indexOf(symbol) == -1 && dictionary.indexOf(symbol) == -1)
            alphabet.append(symbol);
    }
}

void RegExp::create_nfa(QString &string, QMap<QString, QList<node*> > list_brackets_nodes, bool isEnded)
{
    add_sign(string);
    nfa = std::make_unique<NFAuto>(alphabet,isEnded);
    int pos = 0;
    while(string.length() > pos)
    {
        QChar symbol = string[pos];
        if(string.length()>=(pos+3) && (symbol == '/')){
            int number_pos = string.indexOf('/',pos+2) + 1;
            QString str = string.mid(pos,number_pos-pos);
            nfa->addInput_symbols(str);
            nfa->setStack_automat(list_brackets_nodes.find(str).value());
            for(node* _node : list_brackets_nodes.find(str).value()){
                if(_node->get_is_starting())
                   nfa->setStack_begin(_node);
                if(_node->get_is_Ended())
                    nfa->addStack_end(_node);
            }
            pos=number_pos-1;
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
            nfa->addInput_symbols(symbol);
        }
        pos++;
    }
    nfa->setEnd();
}

void RegExp::add_sign(QString& string){
    if(string[0] != '/')
        string.insert(1,'&');
    else{
        int number_pos = string.indexOf('/',2) + 1;
        string.insert(number_pos,'&');
    }
    int pos = 2;
    while(pos < string.length()){
        QChar symbol = string[pos];
        if(symbol == '/'){
            int number_pos = string.indexOf('/',pos+2) + 1;
            string.insert(number_pos,'&');
            pos=number_pos-1;
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

void RegExp::change_plus_by_kleene_and_contagenation(){
    int current_index = 0;
    int plus_index = expression.indexOf(")+",current_index);
    int begin_index = 0;

    while(plus_index != -1){
        int number = 1;
        while(number != 0){
            for(int j = plus_index-1; j>=0; j--){
                if(expression[j] == '(')
                    number--;
                if(expression[j] == ')')
                    number++;
                if(number == 0)
                {
                    begin_index = j;
                    break;
                }
            }
        }
        QString current_string = expression.mid(begin_index,plus_index-begin_index+1);
        expression.remove(plus_index+1,1);
        expression.insert(plus_index+1,"*"+current_string);
        plus_index+= current_string.length()+1;
        current_index = plus_index;
        plus_index = expression.indexOf(")+",current_index);
    }
}

bool RegExp::check_valid_string()
{
    //проверка равенства числа левых и правых скобок
    {
        int index = 0;
        for(QChar symbol:expression){
            if(symbol == '(')
                index++;
            else if(symbol == ')')
                index--;
            if(index < 0){
                description = "Некорректное. Закрывающаяся скобка без открывающейся.";
                isValid = false;
                return false;
            }
        }
        if(index != 0){
            description = "Некорректное. Открывающаяся скобка без закрывающейся.";
            isValid = false;
            return false;
        }
    }
    //проверка равенства числа левых и правых скобок

    QStringList incorrect_symbols_list;
    incorrect_symbols_list << "||" <<"++"<<"**"<<"*+"<<"+*"<<"|+"<<"|*";
    int position = 0;
    for(QString iter : incorrect_symbols_list){
        if(expression.indexOf(iter) != -1)
            break;
        position++;
    }

    if(position != incorrect_symbols_list.length()){
        description = "Некорректное. Два знака "+incorrect_symbols_list[position]+" подряд.";
        isValid = false;
        return false;
    }

    if(expression.contains('/')){
        description = "Некорректное. Запрещенный символ /";
        isValid = false;
        return false;
    }

    description = "Корректное.";
    isValid = true;
    return true;
}


void RegExp::delete_empty_brackets(){
    int current_index = expression.indexOf("()");
    while(current_index!=-1){
        expression.remove(current_index,2);
        current_index = expression.indexOf("()");
    }
}

QString RegExp::getDescription() const
{
    return description;
}

QList<node *> RegExp::getListNFANodes()
{
    return nfa->getNodes();
}

QList<node *> RegExp::getListDFANodes()
{
    return QList<node *>();
}

bool RegExp::getIsValid() const
{
    return isValid;
}

QList<QChar> RegExp::getAlphabet() const
{
    return alphabet;
}
