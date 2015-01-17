#include "liuifile.h"

LIUIFile::LIUIFile(const QString& _fileName, QTextStream& _out):
    LISourceFile(_fileName, _out)
{
}

bool LIUIFile::processMessage(const LIMessage& m)
{
    uint index = m.line-1;
    if (index>=(uint)content.count()) {
        out << tr("linvert error: File %1 doesn't contain line %2\n").arg(fileName).arg(m.line);
        return false;
    }
    if (!content[index].contains(m.oldText)) {
        out << tr("linvert error: File %1 doesn't contain value %2\n").arg(fileName).arg(m.oldText);
        return false;
    }
    content[index] = content[index].replace(m.oldText, m.newText);
    return true;
}
