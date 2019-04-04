#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <regexp.h>
#include "dfa.h"
#include <memory>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_checkButton_clicked();

    void on_comboBox_activated(int index);

    void on_genButton_clicked();

private:
    bool check_valid_string(const QString &string, QString &description);
    void make_alphabet(QString &string);
    QMap<QString, QString> choose_bracket(QString &string);

private:
    Ui::MainWindow *ui;
    QList<QChar> dictionary;
    QList<QChar> alphabet;
    QMap<QString, QList<RegExp::node*> > special_list;
    RegExp* nfa;
    std::unique_ptr<DFA> dfa;
    //void create_nfa(const QString &string);
    int comboBoxIndex;
    void makeNFAfromRegExp();
    void create_nfa(QString &string, QMap<QString, QList<RegExp::node *> > list_brackets_nodes);
    void add_sign(QString &string);
    void change_plus_by_kleene_and_contagenation(QString &string);
};

#endif // MAINWINDOW_H
