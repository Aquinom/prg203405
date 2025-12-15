#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QString>

enum class Language
{
    English,
    Portuguese
};

namespace Localization
{
    inline QString text(Language language, const QString &english, const QString &portuguese)
    {
        return (language == Language::English) ? english : portuguese;
    }
}

#endif // LANGUAGE_H
