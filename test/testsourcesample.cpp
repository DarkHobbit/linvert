// Please don't run lupdate/linvert on this file directly.
// Instead this, use testli1 and (after editing test.ts) testli2 scripts

#include <QObject>
#include <QString>

#define DEF3 "str32"
const QString const4 = "str42";

int main()
{
    // Cyrillic UTF8 strings:
    // First and second examples works correctly.
    QString s1 = QObject::trUtf8( "Строка1" );
    QString s2 = QObject::trUtf8("Prefix2") + QObject::trUtf8( "Строка21" + "Строка22");
    // Examples 3-5 not works. It also not supported by Qt lupdate (only "Строка31/41/51" detected)
/*    QString s3 = QObject::trUtf8( "Строка31" + DEF3 + "Строка33");
    QString s4 = QObject::trUtf8( "Строка41" + const4 + "Строка43");
    QString s5 = QObject::trUtf8( "Строка51" + ("Строка52") + "Строка53"); */
    // Next example works correctly.
    QString s6 = QObject::trUtf8( "Строка61" \
       "Строка62");
    // Further mix - works correctly.
    QString s7 = QObject::trUtf8("Строка71") + QObject::trUtf8("Строка72") + QObject::trUtf8("Строка71" \
     "13");
    return 0;
}

