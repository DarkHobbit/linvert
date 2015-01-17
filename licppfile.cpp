#include "licppfile.h"

LICPPFile::LICPPFile(const QString& _fileName, QTextStream& _out):
    LISourceFile(_fileName, _out)
{
}

bool LICPPFile::processMessage(const LIMessage& m)
{
    int index = m.line-1;
    if (index>=content.count()) {
        out << tr("linvert error: File %1 doesn't contain line %2\n").arg(fileName).arg(m.line);
        return false;
    }
    // String may include some parts
    int pos = 0;
    int bPos, ePos, addLines;
    while (pos<content[index].length()) {
        QString cand = nextTr(index, pos, bPos, ePos, addLines);
/*out << QString("s %1 c %2 b %3 e %4 al %5\n").arg(m.oldText).arg(cand).arg(bPos).arg(ePos).arg(addLines);*/
        if (cand.length()==0) break;
        if (matchTr(cand, m.oldText)) {
            content[index] = content[index].left(bPos)+"\""+m.newText+"\""+content[index+addLines].mid(ePos);
            for (int i=index+1; i<index+addLines+1; i++)
                content[i] = "";
            return true;
        }
        else if (addLines>0) break;
        pos = ePos+1;
    }
    out << tr("linvert error: File %1 doesn't contain value %2\n").arg(fileName).arg(m.oldText);
    return false;
}

// Find next tr unit in line row, including quotes
QString LICPPFile::nextTr(int row, int col, int& bPos, int& ePos, int& addLines)
{
    const QString keyWord = "trUtf8";
    QString src = content[row];
    int pos = src.indexOf(keyWord, col);
    if (pos==-1) return "";
    pos += keyWord.length();
    // Search left parenthesis
    pos = src.indexOf('(', pos);
    if (pos==-1) return "";
    bPos = pos;
    // Search right parenthesis (recursive parentheses not supported, as in lupdate)
    addLines = 0;
    ePos = src.indexOf(')', bPos); // ...in 1st line
    do {
        if (ePos!=-1) {
            // Search left quote (right from bPos)
            bPos = content[row].indexOf('\"', bPos);
            // Search right quote (left from ePos)
            ePos = src.lastIndexOf('\"', ePos-src.length());
            if (ePos==-1) return "";
            ePos++;
            break;
        }
        // Next line
        addLines++;
        src = content[row+addLines];
        ePos = src.indexOf(')', 0); // ...in other lines
    } while (row+addLines<content.count());
    if (ePos==-1) return "";
    // Concatenated string (possibly, from some lines)
    QString res = "";
    for (int i=0; i<=addLines; i++) {
        QString part = content[row+i];
        if (i==addLines) part = part.left(ePos);
        if (i==0) part = part.mid(bPos);
        res +=part;
    }
    return res;
}

bool LICPPFile::matchTr(const QString& candidate, const QString& pattern)
{
    QString s = "";
    bool valuablePart = false;
    for (int i=0; i<candidate.length(); i++) {
        if (candidate[i]=='\"') valuablePart = !valuablePart;
        else
            if (valuablePart) s+=candidate[i];
    }
    return (s==pattern);
}
