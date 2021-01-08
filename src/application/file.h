#pragma once

class QString;
class QStringList;

class File
{
public:
//     File() def;

    static QString readTextFile(const QString& filePath);
    static QStringList readTextLines(const QString& filePath);
    static void writeTextFile(const QString& filePath, const QString& text);
    static void writeTextFile(const QString& filePath, const QStringList& textLines);
};
