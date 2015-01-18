/****************************************************************************
**
** Copyright (C) 2015 Mikhail Y. Zvyozdochkin aka DarkHobbit
** Contact: pub@zvyozdochkin.ru
**
** This file is part of the LInvert utility for Qt
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public License
** version 2.1 or version 3 as published by the Free Software Foundation.
** Please review the ** following information to ensure the
** GNU Lesser General Public License requirements will be met:
** https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

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
