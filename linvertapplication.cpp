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
    QStringList tsFiles;
    for (int i=1; i<arguments().count(); i++) {
        // TODO: Option?
        // Otherwise, it must be a ts file name
        QString fileName = arguments()[i];
        if (fileName.right(3)!=".ts")
            fileName.append(".ts");
        if (!QFile(fileName).exists()) {
            out << tr("linvert error: File %1 not exists\n").arg(fileName);
            return 2;
        }
        tsFiles.push_back(fileName);
    }
    for (int i=0; i<tsFiles.count(); i++)
        if (!processTS(tsFiles[i]))
            return 3;
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
    QDomElement root = ts.documentElement();
    if (root.nodeName()!="TS") {
        out << tr("linvert error: Root node is not 'TS' at file %1\n").arg(fileName);
        return false;
    }
    QDomElement c = root.firstChildElement();
    while (!c.isNull()) {
        if (c.nodeName()=="context") {
            QDomElement msg = c.firstChildElement();
            int msgCount = 0;
            while (!msg.isNull()) {
                if (msg.nodeName()=="name")
                    out << tr("Context: ") << msg.text() << "\n";
                else if (msg.nodeName()=="message") {
                    if (!processMessageNode(fileName, msg))
                        return false;
                    msgCount++;
                }
                else
                    out << tr("linvert  warning: unknown element %1\n").arg(msg.nodeName());
                msg = msg.nextSiblingElement();
            }
            out << tr("Found %1 messages\n").arg(msgCount);
        }
        c = c.nextSiblingElement();
    }
    // TODO: write modified file
    // Bye-bye
    return true;
}

bool LInvertApplication::processMessageNode(const QString& fileName, QDomElement &msg)
{
    QString mSrc = "";
    QString mTr = "";
    // Parse node
    QDomElement ch = msg.firstChildElement();
    while (!ch.isNull()) {
        if (ch.nodeName()=="source")
            mSrc = ch.text();
        else if (ch.nodeName()=="translation")
            mTr = ch.text();
        ch = ch.nextSiblingElement();
    }
    if ((mSrc=="")||(mTr=="")) {
        out << tr(
            "linvert error: Incomplete message at file %1, source: %2.\nProcessing may corrupt source files\n")
               .arg(fileName).arg(mSrc);
        return false;
    }
    out << "Source: " << mSrc << "\n";
    out << "Translation: " << mTr << "\n";
    // Swap source and translation

    // TODO save QDomElements instead string in first loop?
    return true;
}

