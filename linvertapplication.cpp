#include <QFile>
#include <QStringList>
#include "linvertapplication.h"
#include "liuifile.h"
#include "licppfile.h"

LInvertApplication::LInvertApplication(int &argc, char **argv)
  : QCoreApplication(argc, argv),
    out(stdout)
{
}

// Main program loop
int LInvertApplication::start()
{
    out << tr("LInvert for Qt Utility by Mikhail Y. Zvyozdochkin\n");
    // Parse command line
    if (arguments().count()<2) {
        printUsage();
        return 1;
    }
    QStringList tsFiles;
    QString langCode;
    for (int i=1; i<arguments().count(); i++) {
        // Option: language code?
        if (arguments()[i]=="-l") {
            i++;
            if (i==arguments().count()) {
                out << tr("linvert error: -l option present, but lang code is missing\n");
                printUsage();
                return 2;
            }
            langCode = arguments()[i];
            continue;
        }
        // Otherwise, it must be a ts file name
        QString fileName = arguments()[i];
        if (fileName.right(3)!=".ts")
            fileName.append(".ts");
        if (!QFile(fileName).exists()) {
            out << tr("linvert error: File %1 not exists\n").arg(fileName);
            printUsage();
            return 3;
        }
        tsFiles.push_back(fileName);
    }
    if (langCode.length()==0) {
        out << tr("linvert error: -l option is missing\n");
        printUsage();
        return 4;
    }
    srcFiles.clear();
    // Process found TS files
    for (int i=0; i<tsFiles.count(); i++)
        if (!processTS(tsFiles[i], langCode))
            return 5;
    // Process source files
    for (int i=0; i<srcFiles.count(); i++)
        if (!srcFiles[i]->process()) {
            for (int j=0; j<=i; j++)
                srcFiles[j]->rollBack();
            return 6;
        }
    return 0;
}

// Print program usage if error occured
void LInvertApplication::printUsage()
{
    out << tr(
        "Usage:\n" \
        " linvert -l LANG_CODE ts-file [ts-file]...\n"
               );
}

// Process one .ts file
bool LInvertApplication::processTS(const QString& fileName, const QString& langCode)
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
    root.setAttribute("language", langCode);
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
            out << tr("Found %1 messages in context\n").arg(msgCount);
        }
        c = c.nextSiblingElement();
    }
    // Write modified file
    QString outFileName = fileName;
    outFileName.replace(".ts", QString("_%1.ts").arg(langCode));
    QFile outFile(outFileName);
    if (!outFile.open( QIODevice::WriteOnly)) {
        out << tr("linvert error: Can't write file %1\n").arg(outFileName);
        return false;
    }
    QTextStream out(&outFile);
    ts.save(out, 4);
    outFile.close();
    // Bye-bye
    return true;
}

bool LInvertApplication::processMessageNode(const QString& fileName, QDomElement &msg)
{
    QString mSrc = "";
    QString mTr = "";
    QString srcName = "";
    uint srcLine = 0;
    QDomElement eSrc, eTr;
    // Parse node
    // TODO ignore nodes where UTF8 is false (already on Latin1!)
    QDomElement ch = msg.firstChildElement();
    while (!ch.isNull()) {
        if (ch.nodeName()=="source") {
            mSrc = ch.text();
            eSrc = ch;
        }
        else if (ch.nodeName()=="translation") {
            mTr = ch.text();
            eTr = ch;
        }
        else if (ch.nodeName()=="location") {
            srcName = ch.attribute("filename", "");
            srcLine = ch.attribute("line").toUInt();
        }
        ch = ch.nextSiblingElement();
    }
    if ((mSrc=="")||(mTr=="")) {
        out << tr(
            "linvert error: Incomplete message at file %1, source: %2.\nProcessing may corrupt source files\n")
               .arg(fileName).arg(mSrc);
        // TODO maybe here add pseudo-number to untranslated messages?
        return false;
    }
    /*out << "Source: " << mSrc << "\n";
    out << "Translation: " << mTr << "\n";*/
    // Swap source and translation
    msg.removeChild(eSrc);
    msg.removeChild(eTr);
    eSrc = msg.ownerDocument().createElement("source");
    eSrc.appendChild(msg.ownerDocument().createTextNode(mTr));
    msg.appendChild(eSrc);
    eTr = msg.ownerDocument().createElement("translation");
    eTr.appendChild(msg.ownerDocument().createTextNode(mSrc));
    msg.appendChild(eTr);
    msg.removeAttribute("utf8");
    // Mark node for change in source file
    if ((srcName.length()==0)||(srcLine==0)) {
        out << tr(
            "linvert  warning: unknown file or line for message '%1' in file %2\nSource will not modified")
                .arg(mSrc).arg(fileName);
    }
    else {
        LISourceFile* srcFile = srcFiles.findByFileName(srcName);
        if (!srcFile) {
            if (srcName.contains(".ui"))
                srcFile = new LIUIFile(srcName, out);
            else
                srcFile = new LICPPFile(srcName, out);
            srcFiles.push_back(srcFile);
        }
        srcFile->addMessage(mSrc, mTr, srcLine);
    }
    return true;
}

