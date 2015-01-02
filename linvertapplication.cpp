#include <QDomDocument>
#include <QFile>
#include <QStringList>
#include "linvertapplication.h"

LInvertApplication::LInvertApplication(int &argc, char **argv)
  : QCoreApplication(argc, argv),
    out(stdout)
{
}

// Main program loop
int LInvertApplication::start()
{
    out << tr("LInvert for Qt Utility by Mikhail Y. Zvyozdochkin\n");
    if (arguments().count()<2) {
        printUsage();
        return 1;
    }
    for (int i=1; i<arguments().count(); i++) {
        QString fileName = arguments()[i];
        if (fileName.right(3)!=".ts")
            fileName.append(".ts");
        if (!QFile(fileName).exists()) {
            out << tr("linvert error: File %1 not exists\n").arg(fileName);
            return 2;
        }
        if (!processTS(fileName))
            return 3;
    }
    return 0;
}

// Print program usage if error occured
void LInvertApplication::printUsage()
{
    out << tr(
        "Usage:\n" \
        " linvert ts-file [ts-file]...\n"
               );
}

// Process one .ts file
bool LInvertApplication::processTS(const QString& fileName)
{
    out << tr("Processing %1:\n").arg(fileName);
    // Open
    QFile file(fileName);
    if (!file.open( QIODevice::ReadOnly)) {
        out << tr("linvert error: Can't open file %1\n").arg(fileName);
        return false;
    }
    // Read to document
    QDomDocument ts("TS");
    QString err_msg;
    int err_line;
    int err_col;
    if (!ts.setContent(&file, &err_msg, &err_line, &err_col)) {
        out << tr("linvert error: Can't read content from file %1\n" \
            "%2\nline %3, col %4\n"
            ).arg(fileName).arg(err_msg).arg(err_line).arg(err_col);
        file.close();
        return false;
    }
    file.close();
    // Parse
    QDomElement el = ts.documentElement();
    QDomNode n = el.firstChild();
    // Bye-bye
    return true;
}

