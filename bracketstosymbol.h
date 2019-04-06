#ifndef BRACKETSTOSYMBOL_H
#define BRACKETSTOSYMBOL_H

#include <QMap>
#include <regexp.h>

class BracketsToSymbol
{
public:
    BracketsToSymbol(QString & etalon);
    QList<std::pair<QString,QString> > get_bracket_list_priority();
    QMap<QString, QList<node *> > get_bracket_node_map();
    void insert_bracket_node_map(QString &str, QList<node *> list_node);
private:
    QMap<QString, QList<node*> > node_map;
    QList<std::pair<QString,QString> > list_str_by_prioryte;
    void choose_bracket(QString &string);
    void make_list_priority();
};

#endif // BRACKETSTOSYMBOL_H
