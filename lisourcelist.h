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

#ifndef LISOURCELIST_H
#define LISOURCELIST_H

#include <QList>
#include <QString>
#include "lisourcefile.h"

class LISourceList: public QList <LISourceFile*>
{
public:
    LISourceList();
    ~LISourceList();
    LISourceFile* findByFileName(const QString& str);
};

#endif // LISOURCELIST_H
