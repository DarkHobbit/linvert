#include "lisourcefile.h"

const QString BACKUP_SUFFIX = "~";

bool LIMessage::operator ==(const LIMessage& m2)
{
    return (this->oldText==m2.oldText);
}

LISourceFile::LISourceFile(const QString& _fileName, QTextStream& _out):
    fileName(_fileName), out(_out)
{
}

bool LISourceFile::isSameFile(const QString& _fileName)
{
    return fileName==_fileName;
}

bool LISourceFile::addMessage(const QString& _oldText, const QString& _newText, uint _line)
{
    LIMessage item;
    item.oldText = _oldText;
    item.newText = _newText;
    item.line = _line;
    if (!messages.contains(item)) {
        messages.push_back(item);
        return true;
    }
    else
        return false;
}

bool LISourceFile::process()
{
    if (!read()) return false;
    for (int i=0; i<messages.count(); i++)
        if (!processMessage(messages[i]))
            return false;
    return write();
}

void LISourceFile::rollBack()
{
    QFile::remove(fileName);
    QFile::rename(fileName+BACKUP_SUFFIX, fileName);
}

bool LISourceFile::read()
{
    QString bckName = fileName+BACKUP_SUFFIX;
    QFile::remove(bckName); // only if exists
    if (!QFile::rename(fileName, bckName)) {
        out << tr("linvert error: Can't rename file %1\n").arg(fileName);
        return false;
    }
    QFile file(bckName);
    if (!file.open( QIODevice::ReadOnly)) {
        out << tr("linvert error: Can't open file %1\n").arg(bckName);
        return false;
    }
    content.clear();
    QTextStream stream(&file);
    do {
        content.push_back(stream.readLine());
    } while (!stream.atEnd());
    file.close();
    return true;
}

bool LISourceFile::write()
{
    QFile file(fileName);
    if (!file.open( QIODevice::WriteOnly)) {
        out << tr("linvert error: Can't open file %1\n").arg(fileName);
        return false;
    }
    QTextStream stream(&file);
    for (int i=0; i<content.count(); i++)
        stream << content[i] << "\n";
    file.close();
    return true;
}
