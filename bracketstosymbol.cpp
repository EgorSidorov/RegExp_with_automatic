#include "bracketstosymbol.h"

BracketsToSymbol::BracketsToSymbol(QString &etalon)
{
    choose_bracket(etalon);
    make_list_priority();
}

QList<std::pair<QString,QString> > BracketsToSymbol::get_bracket_list_priority()
{
    return list_str_by_prioryte;
}

void BracketsToSymbol::make_list_priority(){

}

void BracketsToSymbol::insert_bracket_node_map(QString& str, QList<RegExp::node *> & list_node){
    node_map.insert(str,list_node);
}

QMap<QString, QList<RegExp::node *> > BracketsToSymbol::get_bracket_node_map()
{
    return node_map;
}

void BracketsToSymbol::choose_bracket(QString &string)
{
    int last_number = 0;
    while(string.indexOf(')') != -1){
        int end_str = string.indexOf(')');
        QString bracket_string = "";
        int begin_str;
        {   int iter = end_str-1;
            while(string[iter]!='('){
                bracket_string.prepend(string[iter]);
                iter--;
            }
            begin_str = iter;
        }
        QString replace_string = "/"+QString::number(last_number)+"/";
        string.replace(begin_str,end_str-begin_str+1,replace_string);
        last_number++;
        list_str_by_prioryte.append(std::pair<QString,QString>(replace_string,bracket_string));
    }
}

