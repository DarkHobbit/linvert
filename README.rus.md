# LInvert #

Утилита для адаптации программ на Qt4/5 к требованиям Qt Linguist.

## Назначение ##

Как известно, правила локализации Qt-приложений требуют, чтобы все строковые константы в программе
были обрамлены в функции tr() или translate(). Также считается хорошей практикой (хотя и не требуется
разработчиками Qt), чтобы эти константы содержали только латинские символы. В противном случае
надо либо заменять tr() на trUtf8(), либо колдовать с кодеками и CODECFORTR/CODECFORSRC. Ситуация
осложняется, когда для национального языка имеют хождение несколько разных кодировок, с русским
языком дела обстоят именно так.

Как правило, в текстах программы все сообщения пишут на английском языке, а остальные языки
существуют только в файлах переводов. Единственный минус такой схемы - если программа изначально
разрабатывалась для неанглоязычных пользователей (особенно это относится к заказному ПО, которое
изначально делается под конкретного заказчика), то размещая английский текст в коде, а перевод
где-то ещё, мы проделываем двойную работу. Поэтому зачастую текст хардкодится в программе
на родном для программистов и пользователей языке (в этом случае крайне рекомендуется
применение UTF-8). Но что делать, если ПО изначально делалось для конкретных клиентов, а потом начало перерастать
в международный продукт?

Описанная ситуация характерна для закрытого коммерческого ПО. Но схожая проблема может возникнуть
и в открытых проектах: программист делал программу "для себя", не особо задумываясь о переводах,
а потом увидел, что его детище может быть полезным ещё кому-то, и захотел выложить исходники
в общий доступ. Если строки в программе не английские, это может осложнить привлечение к проекту
разработчиков из других стран.

Здесь есть два способа. Простейший - оставить всё как есть и файлы
переводов делать "с русского на финский". Linguist такое позволяет, и в формате файла .ts
предусмотрены соответствующие атрибуты. Правда, с отображением контекста перевода в исходниках
из Linguist возможны косяки. Кроме того, найти переводчиков на какой-нибудь
экзотический язык может оказаться проще с английского, чем с русского (особенно для
некоммерческого проекта). Для некоммерческого проекта также важно, что при этом вряд ли
удастся работать с иноязычными программистами.

Второй способ - привести программу к общепринятой практике, то есть заменить все национальные
строковые константы английскими, а национальные вынести в файлы перевода. Если заниматься
этим вручную, это очень трудоёмкая процедура.

Утилита linvert позволяет частично автоматизировать второй способ.

## Технология применения ##

Процесс адаптации программ на Qt4/5 к требованиям Qt Linguist с применением linvert
включает в себя 4 шага:

  * Проводим ревизию исходных текстов. Проверяется, что все файлы программы выполнены в кодировке
UTF8, а все строки, подлежащие переводу, обрамлены в функцию trUtf8. Эти строки не должны
составляться слишком сложным образом, другими словами, их должна понимать утилита lupdate
из Qt Linguist (подробнее об этом в разделе "Ограничения"). Для упрощения унификации кодировок
unix-программисты могут воспользоваться программой enca (о чём я писал в Башне ???).

  * Создаём прототипы файлов перевода. Прототипы - это обычные файлы .ts, но
теги source и translation у него "перепутаны". Создаются обычной командой lupdate из состава
Qt Linguist.

  * Переводим файлы .ts на английский с помощью Linguist. Переводчикам (если это отдельные люди)
надо объяснить, что национальный текст будет в графе "Исходный текст", а переведённый - в графе
"Русский перевод", и это нормально. Если часть сообщений уже была на английском языке, их можно
перевести обратно.

    * По окончании этого шага можно пометить сообщения, которые уже были на английском языке,
чтобы linvert их не переводил. Для этого в элемент <message> добавляется атрибут linvert="false".
До вызова Linguist это делать смысла нет, поскольку Linguist такого атрибута не знает и
просто удалит его при перезаписи файла. Атрибут linvert="true", наоборот, сообщит, что
сообщение надо переводить, даже если программа вызвана с ключом -i (см. "Вызов программы").

  * Теперь можно осуществить собственно замену, вызвав linvert и указав ей, какие .ts-файлы
следует переводить. Программа создаст новый .ts-файл с суффиксом соответствующего языка,
  TODO код внутрь файла. 
а также заменит в исходных файлах (*.ui, *.h, *.cpp) национальные сообщения на английские.

На этом адаптация закончена. Можно включать полученный .ts в проект, загружать .qm в программу
и переводить проект на другие языки.

## Вызов программы ##

Как lupdate и lrelease, linvert -  утилита командной строки. Вызов выглядит следующим образом:

linvert -l LANG_CODE [-p] [-i] ts-file [ts-file]...

Ключ -l задаёт код национального языка, который будет использован в качестве суффикса для имён
выходных .ts-файлов.

Ключи -p и -i определяют поведение программы, если для некоторого элемента message отсутствует
атрибут linvert. Ключ -p предписывает обрабатывать такие записи (поведение по умолчанию), ключ
-i - игнорировать.

## Ограничения ##

В текущей версии linvert поддерживается преобразование файлов исходного кода C++ и файлов *.ui.
Поддержки QML пока нет.

Программа linvert накладывает на файлы исходного кода те же ограничения, что и программа lupdate
из состава Qt Linguist.

Поддерживаются:
  * простая конкатенация строк;
  * перенос строковой константы на следующую строку исходного кода (обратный слэш);
  * несколько конструкций trUtf8() в одной строке исходного кода.
  
НЕ поддерживаются:
  * определения #define и поименованные строковые константы, включаемые в trUtf8();
  * вложенные круглые скобки.

К примеру, в закомментированной части файла test/testsourcesample.cpp из выражения,
присваиваемого строке s3, lupdate определит только фрагмент "Строка31". Соответственно,
если у Вас в строковых операциях фигурирует макроопределение #define национальном языке,
в trUtf8 надо обрамлять само макроопределение, а не его использование.

## Условия распространения ##

Программа распространяется по лицензии LGPL 2.1 либо LGPL 3, так же, как и утилиты
lupdate и lrelease из состава Qt Linguist.