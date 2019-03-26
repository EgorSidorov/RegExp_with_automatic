#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <nfa.h>

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

private:
    bool check_valid_string(const QString &string, QString &description);
    void write_dictionary(const QString &string, QList<QChar>& non_terminal_value);
    void choose_bracket(QString &string, QStringList& list_special_string);

private:
    Ui::MainWindow *ui;
    QList<QChar> dictionary;
    QList<QChar> non_terminal;
    NFA* nfa;
    void create_nfa(const QString &string);
};

#endif // MAINWINDOW_H
