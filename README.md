# LInvert #

Utility for adopt non-linguist Qt program on national languages to Qt Linguist requirements.

This manual also available on [Russian](./README.rus.md).

## Intro ##

As you know, the rules of Qt-applications internationalization require decorate all string constants in program to tr () or translate (). Good practice (though not required the developers of Qt), when these constants contain only Latin characters. Otherwise developer must either replace the tr () on trUtf8 (), or to conjure with codecs and CODECFORTR / CODECFORSRC. Problem complicated when there some different encodings for one language (i.e. in Russian).

In most cases, all messages in Qt-based application sources written on English. Othes languages appears only in translation files. This scheme is beautiful, but has ONE disadvantage. If software originally written for non-English users (primarily this applies to custom software), presence both English text and translation is a double work for developer. So, often messages hardcoded in source on native language. In this case, strongly recommended use UTF-8. But what can we do, if software primarily developed for specific customer, and then start to grow
in international product?

Problem described above is typical for closed-source (primarily, custom) software. But in some cases it may cause in opensource project: developer wrote application for himself, and did not think about i18n and translations. Then, he realized that his "child" may be useful for someone in world. :) He opened the source code. And non-English messages may complicate attracting developers from other countries.

There ase two ways to solve this problem.

First (simply way) is not modify the sources, but create traslations "from Swedish to Norwegian". Qt Linguist allows this way, and .ts-files has appropriate attributes. But Linguist GUI may incorrectly show translation context. Also, for some exotic languages may be difficult find translators from non-English to this exotic language. For non-commercial project, foreign language developers may have difficulty in project.

Second way - replace all national strings to English, and move national strings to translation files. Manyally, it is a very difficult procedure.

LInvert allows you to automate part of the work at second way.

##Adopt technology##

Adopt process of Qt4/5 application with use of LInvert includes 4 steps:

  * verify sources. All sources must be in UTF-8, all translating strings must be decorated in trUtf8. This strings must NOT be too complex (must be recognized by lupdate tool, for more detais see "Restrictions" section). To unify source encoding, unix developers can use [enca utility](http://packages.ubuntu.com/ca/source/precise/enca).
 
  * create translation prototypes. Prototypes is a simple .ts files, but it has "swapped" *source* and *translation* elements. Prototypes are creation by lupdate utility from Qt Linguist.
 
  * translate .ts files to English, using Linguist. You must say translators, that national text is in source string list, and English text is in translation editor, and it's normal.
 
    * At end of this step you can mark already non-swapped messages to prevent linvert process it. Simply add to *message* element attribute
    linvert="false". DO NOT MAKE IT before Linguist call, because Linguist don't
    know this attribute and it will simply deleted.
    Attribute linvert="true", on the contrary, says LInvert
    always process message, even if LInvert called with -i option (see Syntax).
    
  * call Linvert and say it list files proceeded. Utility will create new .ts file with appropriate language suffix and replace national messages to English in source files (*.ui, *.h, *.cpp).
  
That's all! You can include result .ts file in project, load .qm in your application and translate project in other languages.
  
## Syntax ##

Like lupdate and lrelease, linvert is a command-line utility. Syntax:

  linvert -l LANG_CODE [-p] [-i] ts-file [ts-file]...

-l options sets language code (for example, ru_RU), which LInver use as a suffix for output files, also in language attribute of TS element.

-p and -i options sets default LInvert behaviour, if for any message element linvert attribute is absent: -p to process such records (default), -i to ignore it.

## Restrictions ##

Current LInvert version support C++ source files and .ui files. QML still not supported.

LInvert imposes the same restrictions as lupdate utility.

LInvert support:
  * simple strings concat;
  * transfer to the next line (backslash);
  * some trUtf8() calls in one source line.

LInvert NOT support:
  * #define macro and string constants inside trUtf8();
  * nested parentheses.

For example, in commented part of test/testsourcesample.cpp, in expression for s3 variable, LInvert will detect ONLY part "Строка31". Therefore, if you use #define with national message, we must use trUtf8() in #define, and not in string, which using pre-defined macro.

## License ##

LInvert may be used under the terms of the GNU Lesser General Public License version 2.1 or version 3, like lupdate and lrelease.

