#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPair>
#include <QFileDialog>
#include <QMessageBox>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "baseutils.h"

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

    void on_tableWidget_itemChanged();

    void on_langBox_currentTextChanged(const QString &arg1);

    void on_saveButton_clicked();

    void on_loadButton_clicked();

private:
    Ui::MainWindow *ui;
    TDictionary dictionary;
    QString prevLang;

    // Graphic functions
    void setEnabledElements(bool isEnable);
    void initLangBox(QSet<QString>& langs);

};
#endif // MAINWINDOW_H
