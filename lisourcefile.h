#ifndef LISOURCEFILE_H
#define LISOURCEFILE_H

#include <QFile>
#include <QStringList>
#include <QTextStream>

struct LIMessage {
    QString oldText; // non-english
    QString newText; // english
    uint line;
    bool operator ==(const LIMessage& m2);
};

class LISourceFile: public QObject
{
public:
    LISourceFile(const QString& _fileName, QTextStream& _out);
    bool isSameFile(const QString& _fileName);
    bool addMessage(const QString& _oldText, const QString& _newText, uint _line);
    bool process();
    void rollBack();
protected:
    QString fileName;
    QTextStream& out;
    QStringList content;
    virtual bool processMessage(const LIMessage& m)=0;
private:
    QList <LIMessage> messages;
    bool read();
    bool write();
};

#endif // LISOURCEFILE_H
