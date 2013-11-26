#ifndef	_HAVE_EXCEL_H__
#define	_HAVE_EXCEL_H__

extern	"C" {
#include "php.h"
}

#include "ExcelFormat.hpp"

using namespace ExcelFormat;

struct excel_object {
	zend_object			std;
	BasicExcel*			pExcel;
    XLSFormatManager*	pFmtMgr;
    char*               szCharset;
};

void	excel_free_storage(void* object TSRMLS_DC);
zend_object_value	excel_create_handler(zend_class_entry* type TSRMLS_DC);
void	register_excel_class_ce(TSRMLS_D);

PHP_METHOD(Excel, __construct);
PHP_METHOD(Excel, open);
PHP_METHOD(Excel, create);
PHP_METHOD(Excel, getWorkSheet);
PHP_METHOD(Excel, getCharset);
PHP_METHOD(Excel, getTotalWorkSheets);
PHP_METHOD(Excel, saveAs);

#endif
