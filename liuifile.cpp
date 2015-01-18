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

#include "liuifile.h"

LIUIFile::LIUIFile(const QString& _fileName, QTextStream& _out):
    LISourceFile(_fileName, _out)
{
}

bool LIUIFile::processMessage(const LIMessage& m)
{
    uint index = m.line-1;
    if (index>=(uint)content.count()) {
        out << tr("linvert error: File %1 doesn't contain line %2\n").arg(fileName).arg(m.line);
        return false;
    }
    if (!content[index].contains(m.oldText)) {
        out << tr("linvert error: File %1 doesn't contain value %2\n").arg(fileName).arg(m.oldText);
        return false;
    }
    content[index] = content[index].replace(m.oldText, m.newText);
    return true;
}
