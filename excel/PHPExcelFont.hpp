#ifndef PHPEXCELFONT_HPP
#define PHPEXCELFONT_HPP

extern "C" {
#include "php.h"
}

#include "ExcelFormat.hpp"

using namespace ExcelFormat;

struct excel_font_object {
    zend_object std;
    ExcelFont*  pExcelFont;
};

void   excel_font_free_storage(void* object TSRMLS_DC);
zend_object_value   excel_font_create_handler(zend_class_entry* type TSRMLS_DC);
void    register_excel_font_class_ce(TSRMLS_D);

PHP_METHOD(ExcelFont, __construct);
PHP_METHOD(ExcelFont, setFontSize);
PHP_METHOD(ExcelFont, setWeight);
PHP_METHOD(ExcelFont, setColor);
PHP_METHOD(ExcelFont, setItalic);
PHP_METHOD(ExcelFont, setUnderline);
PHP_METHOD(ExcelFont, setFontName);

#endif // PHPEXCELFONT_HPP
