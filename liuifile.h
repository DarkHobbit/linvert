#ifndef LIUIFILE_H
#define LIUIFILE_H

#include "lisourcefile.h"

class LIUIFile : public LISourceFile
{
public:
    LIUIFile(const QString& _fileName, QTextStream& _out);
protected:
    virtual bool processMessage(const LIMessage& m);
};

#endif // LIUIFILE_H
