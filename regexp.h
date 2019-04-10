#ifndef NFA_H
#define NFA_H

#include <QList>
#include <QMultiMap>
#include <QTableWidget>
#include <QStack>
#include <node.h>
#include <nfauto.h>
#include <dfa.h>
#include <memory>

class RegExp
{
public:
    RegExp(QString const& str);
    void validate(QString const& str);
public:
    QString getDescription() const;
    QList<node*> getListNFANodes();
    QList<node*> getListDFANodes();

    bool getIsValid() const;

    bool Check(QString const& string);

    QList<QChar> getAlphabet() const;

private:
    QList<QChar> dictionary;
    QList<QChar> alphabet;
    bool isValid;
    void delete_empty_brackets();
    QString expression;
    QString description;
    std::unique_ptr<NFAuto> nfa;
    std::unique_ptr<DFAuto> dfa;
    bool check_valid_string();
    void make_alphabet();
    void add_sign(QString &string);
    void change_plus_by_kleene_and_contagenation();
    void create_nfa(QString &string, QMap<QString, QList<node *> > list_brackets_nodes, bool isEnded);
    void create_dfa();
    void add_unique_nodes(QList<node *> &list, QList<node *>& elements);
};

#endif // NFA_H
