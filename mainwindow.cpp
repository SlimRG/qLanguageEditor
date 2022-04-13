#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create table view
    ui->tableWidget->setColumnCount(2);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_createButton_clicked()
{
    // Get language list
    QSet<QString> langs = LangUtils::GetAllLangs();

    // Init langBox
    foreach (QString lang, langs)
        ui->langBox->addItem(lang);
    ui->langBox->model()->sort(0);
    ui->langBox->setEnabled(true);
    ui->addButton->setEnabled(true);
}


void MainWindow::on_addButton_clicked()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
}


void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem* item)
{
    // Block table
    ui->tableWidget->blockSignals(true);

    // Set all backgrounds to white
    for (int x = 0; x < ui->tableWidget->columnCount(); x++)
        for (int y = 0; y < ui->tableWidget->rowCount(); y++)
        {
            QTableWidgetItem* tmpItem = ui->tableWidget->item(x, y);
            if (tmpItem)
                tmpItem->setBackground(Qt::white);
        }

    // Find current row
    int x = item->row();

    QTableWidgetItem* nameObj = ui->tableWidget->item(x, 0);
    QTableWidgetItem* valueObj = ui->tableWidget->item(x, 1);

    // Color elements and check nulls
    if (!nameObj)
    {
        ui->tableWidget->blockSignals(false);
        return;
    }

    //

    // Check current row
    QString nameStr = nameObj->text();
    QString valueStr = (valueObj) ? valueObj->text() : "";
    QString langStr = ui->langBox->currentText();

    if (nameStr.isEmpty() || langStr.isEmpty())
    {
        ui->tableWidget->blockSignals(false);
        return;
    }

    // Check dublicates of name
    QList<QTableWidgetItem*> sameItems = ui->tableWidget->findItems(nameStr, Qt::MatchCaseSensitive);
    for(QTableWidgetItem* foundItem: sameItems)
    {
        if ((foundItem != nameObj) && (foundItem->column() == 0))
        {
            nameObj->setBackground(Qt::red);
            if (valueObj)
                valueObj->setBackground(Qt::red);
            ui->tableWidget->blockSignals(false);
            return;
        }
    }

    // Set translation
    this->dictionary[langStr][nameStr] = valueStr;

    // Unblock signals
    ui->tableWidget->blockSignals(false);
}


void MainWindow::on_tableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    ui->deleteButton->setEnabled(current);

}

