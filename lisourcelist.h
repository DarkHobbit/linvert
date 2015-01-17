#ifndef LISOURCELIST_H
#define LISOURCELIST_H

#include <QList>
#include <QString>
#include "lisourcefile.h"

class LISourceList: public QList <LISourceFile*>
{
public:
    LISourceList();
    ~LISourceList();
    LISourceFile* findByFileName(const QString& str);
};

#endif // LISOURCELIST_H
