dnl $Id$
dnl config.m4 for extension excel

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(excel, for excel support,
Make sure that the comment is aligned:
[  --with-excel             Include excel support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(excel, whether to enable excel support,
dnl Make sure that the comment is aligned:
dnl [  --enable-excel           Enable excel support])

if test "$PHP_EXCEL" != "no"; then
  dnl Write more examples of tests here...
  dnl compiler C++:
PHP_REQUIRE_CXX()
  dnl # --with-excel -> check with-path
  dnl SEARCH_PATH="/usr/local /usr "     # you might want to change this
  dnl SEARCH_FOR="/include/excel.h"  # you most likely want to change this
  dnl if test -r $PHP_EXCEL/$SEARCH_FOR; then # path given as parameter
  dnl   EXCEL_DIR=$PHP_EXCEL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for excel files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       EXCEL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$EXCEL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the excel distribution])
  dnl fi

  dnl # --with-excel -> add include path
  dnl PHP_ADD_INCLUDE($EXCEL_DIR/include)

  dnl # --with-excel -> check for lib and symbol presence
  dnl LIBNAME=excel # you may want to change this
  dnl LIBSYMBOL=excel # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $EXCEL_DIR/lib, EXCEL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_EXCELLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong excel lib version or lib not found])
  dnl ],[
  dnl   -L$EXCEL_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(EXCEL_SHARED_LIBADD)
 dnl Check for stdc++
  LIBNAME=stdc++
  AC_MSG_CHECKING([for stdc++])
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE(
  [
     #include <string>
    using namespace std;
  ],[
    string dummy;
  ],[
    AC_MSG_RESULT(yes)
    PHP_ADD_LIBRARY($LIBNAME, , EXCEL_SHARED_LIBADD)
  ],[
    AC_MSG_ERROR([wrong stdc++ library not found])
 ])
AC_LANG_RESTORE

PHP_SUBST(EXCEL_SHARED_LIBADD)

dnl Sources
  PHP_NEW_EXTENSION(excel, excel.cpp BasicExcel.cpp ExcelFormat.cpp PHPExcel.cpp PHPExcelFont.cpp PHPExcelFormat.cpp PHPExcelSheet.cpp Util.cpp utf8charsetconvertor.cpp gbkcharsetconvertor.cpp, $ext_shared)
fi
