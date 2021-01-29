#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QStringList>

#include "file.h"


QString File::readTextFile(const QString &filePath)
{
    QString content("");
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        content = textStream.readAll();
        file.close();
    }
    else
    {
        qWarning() << "Failed to open" << filePath << ":" << file.errorString();
    }

    return content;
}

QStringList File::readTextLines(const QString &filePath)
{
    QStringList lines;
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        while (!textStream.atEnd()) {
            lines << textStream.readLine();
        }
        file.close();
    }
    else
    {
        qWarning() << "Failed to open" << filePath << ":" << file.errorString();
    }

    return lines;
}

void File::writeTextFile(const QString& filePath, const QString& text)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        textStream << text;
        file.close();
    }
}

void File::writeTextFile(const QString& filePath, const QStringList& textLines)
{
    if (textLines.length() == 0)
        return;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream textStream(&file);
        for (auto& line : textLines)
        {
            textStream << line << "\n";
        }
        file.close();
    }
}
