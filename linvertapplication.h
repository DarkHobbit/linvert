#ifndef LINVERTAPPLICATION_H
#define LINVERTAPPLICATION_H

#include <iostream>
#include <QCoreApplication>
#include <QTextStream>

class LInvertApplication : public QCoreApplication
{
public:
    LInvertApplication(int &argc, char **argv);
    int start();
    void printUsage();
private:
    QTextStream out;
    bool processTS(const QString& fileName);
};

#endif // LINVERTAPPLICATION_H
