#ifndef LINVERTAPPLICATION_H
#define LINVERTAPPLICATION_H

#include <iostream>
#include <QCoreApplication>
#include <QDomDocument>
#include <QTextStream>
#include "lisourcefile.h"
#include "lisourcelist.h"

class LInvertApplication : public QCoreApplication
{
public:
    LInvertApplication(int &argc, char **argv);
    int start();
    void printUsage();
private:
    QTextStream out;
    bool ignoreDefault;
    LISourceList srcFiles;
    bool processTS(const QString& fileName, const QString& langCode);
    bool processMessageNode(const QString& fileName, QDomElement& msg);
};

#endif // LINVERTAPPLICATION_H
