#ifndef LICPPFILE_H
#define LICPPFILE_H

#include "lisourcefile.h"

class LICPPFile : public LISourceFile
{
public:
    LICPPFile(const QString& _fileName, QTextStream& _out);
protected:
    virtual bool processMessage(const LIMessage& m);
private:
    QString nextTr(int row, int col, int& bPos, int& ePos, int& addLines);
    bool matchTr(const QString& candidate, const QString& pattern);
};

#endif // LICPPFILE_H
