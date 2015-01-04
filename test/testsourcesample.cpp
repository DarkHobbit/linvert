// Please don't run lupdate/linvert on this file directly.
// Instead this, use testli1 and (after editing test.ts) testli2 scripts

#include <QObject>
#include <QString>

#define DEF3 "str32"

int main()
{
    // Cyrillic UTF8 strings:
    // First and second examples works correctly.
    QString s1 = QObject::trUtf8( "Строка1" );
    QString s2 = QObject::trUtf8("Prefix2") + QObject::trUtf8( "Строка21" + "Строка22");
    // Third example not works. It also not supported by Qt lupdate.
    QString s3 = QObject::trUtf8( "Строка31" + DEF3 + "Строка33");
    // Fourth example works correctly.
    QString s4 = QObject::trUtf8( "Строка41" \
       "Строка42");
    return 0;
}

