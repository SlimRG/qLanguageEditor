#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

#include "langutils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_createButton_clicked();

    void on_addButton_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem* item);

    void on_tableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

private:
    Ui::MainWindow *ui;

    QMap<QString, QMap<QString, QString>> dictionary;

};
#endif // MAINWINDOW_H
