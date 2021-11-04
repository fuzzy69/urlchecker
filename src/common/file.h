#pragma once

#include <QString>
#include <QStringList>


namespace common
{
namespace filesystem
{
/// File handling helper functions
class File
{
public:
    /// Reads a text file from given file path and returns file content
    static QString readTextFile(const QString& filePath);
    /// Reads a text file from given file path and returns list of file content lines
    static QStringList readTextLines(const QString& filePath);
    /// Writes text to a file in given file path
    static void writeTextFile(const QString& filePath, const QString& text);
    /// Writes text from given list of text lines to a file in given file path
    static void writeTextFile(const QString& filePath, const QStringList& textLines);
};
}
}
