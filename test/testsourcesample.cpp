// Please don't run lupdate/linvert on this file directly.
// Instead this, use testli1 and (after editing test.ts) testli2 scripts

#include <QObject>
#include <QString>

#define DEF3 "str32"

int main()
{
    // Cyrillic UTF8 strings
    QString s1 = QObject::trUtf8( "Строка1" );
    QString s2 = QObject::trUtf8("Prefix2") + QObject::trUtf8( "Строка21" + "Строка22");
    // Third example not works. It also not supported by Qt lupdate.
    QString s3 = QObject::trUtf8( "Строка31" + DEF3 + "Строка33");
    return 0;
}

