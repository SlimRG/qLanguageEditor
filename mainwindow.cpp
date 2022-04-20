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
    QSet<QString> langs = BaseUtils::GetAllLangs();

    // Clean elements
    this->dictionary.clear();
    ui->tableWidget->clear();

    // Init langBox
    this->initLangBox(langs);
}


void MainWindow::on_addButton_clicked()
{
    // Block table
    ui->tableWidget->blockSignals(true);

    // Create row
    BaseUtils::AddRow(*ui->tableWidget);

    // Unblock table
    ui->tableWidget->blockSignals(false);
}


void MainWindow::on_tableWidget_itemChanged()
{
    // Block table
    ui->tableWidget->blockSignals(true);

    // Set all backgrounds to white
    BaseUtils::ColorTable((*ui->tableWidget), Qt::white);

    // Check dublicates of name
    BaseUtils::ColorDuplicatesInTable((*ui->tableWidget), Qt::red);

    // Unblock signals
    ui->tableWidget->blockSignals(false);
}


void MainWindow::on_langBox_currentTextChanged(const QString &langStr)
{
    // Block table
    ui->tableWidget->blockSignals(true);

    // Save translations
    BaseUtils::SaveTranslations(this->dictionary,
                                (*ui->tableWidget),
                                this->prevLang);

    // Sync table
    BaseUtils::SyncTable(this->dictionary,
                         (*ui->tableWidget),
                         langStr);

    // Set prev lang
    this->prevLang = ui->langBox->currentText();

    // Unblock table
    ui->tableWidget->blockSignals(false);
}


void MainWindow::on_saveButton_clicked()
{
    // Save translations
    BaseUtils::SaveTranslations(this->dictionary,
                                (*ui->tableWidget),
                                this->prevLang);

    // File save dialog
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Save file",
                                                    "",
                                                    "Language file (*.langz)");

    // Create file for write
    QFile* langFile = BaseUtils::OpenFile(filePath, QIODevice::WriteOnly);
    if (!langFile->isOpen())
        return;

    // Create JSON
    QJsonDocument jsonDoc;

    // Serialize translation
    BaseUtils::SerializeJSON(jsonDoc, this->dictionary);

    // Compress
    QByteArray compressedStr = qCompress(jsonDoc.toJson(QJsonDocument::Compact));

    // Rec file and close
    langFile->write(compressedStr);
    BaseUtils::CloseFile(langFile);

}

void MainWindow::on_loadButton_clicked()
{
    // File load dialog
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Load file",
                                                    "",
                                                    "Language file (*.langz *.*)");

    // Create file for write
    QFile* langFile = BaseUtils::OpenFile(filePath, QIODevice::ReadOnly);
    if (!langFile->isOpen())
    {
        this->setEnabledElements(false);
        return;
    }

    // Decompress and close file
    QByteArray jsonInput = qUncompress(langFile->readAll());
    BaseUtils::CloseFile(langFile);

    // Load json
    this->dictionary.clear();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonInput);
    BaseUtils::ParseJSON(jsonDoc, this->dictionary);

    // Get language list
    QSet<QString> langs = BaseUtils::GetAllLangs();
    BaseUtils::AddDicToSet(this->dictionary, langs);

    // Init langBox
    this->initLangBox(langs);
}

/* //
///// GRAPHIC FUNCTIONS
*/ //
void MainWindow::setEnabledElements(bool isEnable)
{
    if (!isEnable)
        ui->tableWidget->setRowCount(0);
    ui->saveButton->setEnabled(isEnable);
    ui->addButton->setEnabled(isEnable);
    ui->langBox->setEnabled(isEnable);
}

void MainWindow::initLangBox(QSet<QString>& langs)
{
    ui->langBox->setEnabled(true);
    ui->langBox->clear();

    foreach (QString lang, langs)
        ui->langBox->addItem(lang);
    ui->langBox->model()->sort(0);
    ui->langBox->setCurrentIndex(0);

    ui->addButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
}

