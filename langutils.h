#ifndef LANGUTILS_H
#define LANGUTILS_H

#include <QDebug>
#include <QList>
#include <QLocale>
#include <QSet>

class LangUtils
{
public:
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
};

#endif // LANGUTILS_H
