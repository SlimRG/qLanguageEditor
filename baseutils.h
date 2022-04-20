#ifndef BASEUTILS_H
#define BASEUTILS_H

#include <QDebug>
#include <QList>
#include <QLocale>
#include <QSet>
#include <QMap>
#include <QFile>
#include <QMessageBox>
#include <QTableWidget>
#include <QSetIterator>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

typedef QMap<QString, QMap<QString, QString>> TDictionary;

class BaseUtils
{
public:

    /* //
     * //   LANGUAGE FUNCTIONS
    */ //

    // Get language list
    static QSet<QString> GetAllLangs()
    {
        auto allLangs = QLocale::matchingLocales(QLocale::AnyLanguage,
                                                 QLocale::AnyScript,
                                                 QLocale::AnyCountry);
        QSet<QString> iso639Languages;

        for (const QLocale& locale : allLangs)
            if (locale.name().contains('_'))
                iso639Languages.insert(locale.name());

        return iso639Languages;
    }  

    // Add QSet to TDictionary
    static void AddDicToSet(TDictionary& dic, QSet<QString>& set)
    {
        QMapIterator<QString, QMap<QString, QString>> dicIter(dic);
        while (dicIter.hasNext())
        {
            dicIter.next();
            set.insert(dicIter.key());
        }
    }

    /* //
     * //   FILESYSTEM FUNCTIONS
    */ //

    // Create or open file
    static QFile* OpenFile(const QString& filePath, QIODevice::OpenMode mode)
    {
        QFile* langFile = new QFile(filePath);
        if (!langFile->open(mode))
        {
            QMessageBox msgBox;
            QString d = (mode == QIODevice::ReadOnly) ? "open" : "create";
            msgBox.setText("Can't "+d+" this file");
            msgBox.setInformativeText("Please, use another file");
            msgBox.exec();
        }
        return langFile;
    }

    // Close file
    static void CloseFile(QFile* file)
    {
        file->close();
        delete file;
    }

    /* //
     * //   JSON FUNCTIONS
    */ //

    // Parse JSON translations
    static bool ParseJSON(const QJsonDocument& jDoc, TDictionary& dic)
    {
        QJsonObject jDic = jDoc.object();
        for (auto jDicIter = jDic.constBegin(); jDicIter != jDic.constEnd(); jDicIter++)
        {
            QJsonArray jTrans = jDicIter.value().toArray();
            for (QJsonValueRef transIter : jTrans)
            {
                QJsonObject jObj = transIter.toObject();
                for (auto valueObj = jObj.constBegin(); valueObj != jObj.constEnd(); valueObj++)
                    dic[jDicIter.key()][valueObj.key()] = valueObj.value().toString();
            }
        }

        return true;
    }

    // Serialize translation in JSON
    static void SerializeJSON(QJsonDocument& jDoc, TDictionary& dic)
    {
        // Serialize translation
        QJsonObject jDic;
        QMapIterator<QString, QMap<QString, QString>> jDicIter(dic);
        while (jDicIter.hasNext())
        {
            jDicIter.next();
            // Create array of translation
            QJsonArray jTrans;
            QMapIterator<QString, QString> transIter(jDicIter.value());
            while (transIter.hasNext())
            {
                transIter.next();

                QJsonObject jsonObj;
                jsonObj.insert(transIter.key(), transIter.value());
                jTrans.push_back(jsonObj);
            }
            if (!jDicIter.key().isEmpty())
                jDic.insert(jDicIter.key(), jTrans);
        }
        jDoc.setObject(jDic);
    }

    /* //
     * //   TABLE WIDGET FUNCTIONS
    */ //

    // Save translations from table widget
    static void SaveTranslations(TDictionary& dictionary, QTableWidget& tw, const QString& lang)
    {
        dictionary[lang].clear();
        for (int x = 0; x < tw.rowCount(); x++)
        {
            QTableWidgetItem* name = tw.item(x, 0);
            QTableWidgetItem* value = tw.item(x, 1);

            if (!name ||name->text().isEmpty())
                continue;

            if (!dictionary[lang].contains(name->text()))
                dictionary[lang][name->text()] = value ? value->text() : "";
        }
    }

    // Synchronize translations from TDictionary
    static void SyncTable(TDictionary& dictionary, QTableWidget& tw, const QString& lang)
    {
        // Resize table
        tw.setRowCount(dictionary[lang].count());

        // Load rows
        if (true)
        {
            QMapIterator<QString, QString> it(dictionary[lang]);
            int x = 0;
            while (it.hasNext())
            {
                it.next();
                tw.setItem(x, 0, new QTableWidgetItem(it.key()));
                tw.setItem(x, 1, new QTableWidgetItem(it.value()));
                x++;
            }
        }
    }

    // Set all backgrounds to white
    static void ColorTable(QTableWidget& tw, const QBrush& brush)
    {
        for (int x = 0; x < tw.rowCount(); x++)
            for (int y = 0; y < tw.columnCount(); y++)
            {
                QTableWidgetItem* tmpItem = tw.item(x, y);
                if (tmpItem)
                    tmpItem->setBackground(brush);
            }
    }

    // Color duplicates in table
    static void ColorDuplicatesInTable(QTableWidget& tw, const QBrush& brush)
    {
        for (int x1 = 0; x1 < tw.rowCount(); x1++)
        {
            if (!tw.item(x1,0) || tw.item(x1,0)->text().isEmpty())
                continue;

            for (int x2 = x1+1; x2 < tw.rowCount(); x2++)
            {
                if (!tw.item(x2,0) || tw.item(x2,0)->text().isEmpty())
                    continue;

                if (tw.item(x1,0)->text() == tw.item(x2,0)->text())
                {
                    if (tw.item(x1,0))
                        tw.item(x1,0)->setBackground(brush);
                    if (tw.item(x1,1))
                        tw.item(x1,1)->setBackground(brush);
                    if (tw.item(x2,0))
                        tw.item(x2,0)->setBackground(brush);
                    if (tw.item(x2,1))
                        tw.item(x2,1)->setBackground(brush);

                    break;
                }
            }
        }
    }

    static void AddRow(QTableWidget& tw)
    {
        tw.setRowCount(tw.rowCount()+1);
        tw.setItem(tw.rowCount()-1, 0, new QTableWidgetItem());
        tw.setItem(tw.rowCount()-1, 1, new QTableWidgetItem());
    }

};

#endif // BASEUTILS_H
