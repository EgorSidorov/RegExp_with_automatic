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
    Ui::MainWindow *ui;
    std::unique_ptr<RegExp> regExp;
    int comboBoxIndex;
    void print();
    void print(QTableWidget *widget);
};

#endif // MAINWINDOW_H
