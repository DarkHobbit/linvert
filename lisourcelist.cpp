#include "lisourcelist.h"

LISourceList::LISourceList()
{
}

LISourceList::~LISourceList()
{
    for (int i=0; i<count(); i++)
        delete at(i);
}

LISourceFile* LISourceList::findByFileName(const QString& str)
{
    LISourceFile* srcFile = 0;
    for (int i=0; i<count(); i++)
    if (at(i)->isSameFile(str)) {
        srcFile = at(i);
        break;
    }
    return srcFile;
}
