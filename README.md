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

 * source revision. All sources must be in UTF-8, all translating strings must be decorated in trUtf8. This strings must NOT be too complex (must be recognized by lupdate tool, for more detais see "Restrictions" section). To unify source encoding, unix developers can use [enca utility](http://packages.ubuntu.com/ca/source/precise/enca).
 
 * create translation prototypes. Prototypes is a simple .ts files, but it has "swapped@ <source> and <translation> elements.
 
 ...

Under construction
