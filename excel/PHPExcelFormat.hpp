#ifndef	_HAVE_PHPEXCEL_FORMAT_H__
#define	_HAVE_PHPEXCEL_FORMAT_H__

extern "C" {
#include "php.h"
}

#include "ExcelFormat.hpp"

using namespace ExcelFormat;

struct cell_format_object {
	zend_object	std;
	CellFormat*	pCellFormat;
};

void	cell_format_free_storage(void* object TSRMLS_DC);
zend_object_value	cell_format_create_handler(zend_class_entry* type TSRMLS_DC);
void	register_cell_format_class_ce(TSRMLS_D);

PHP_METHOD(ExcelCellFormat, __construct);
PHP_METHOD(ExcelCellFormat, setBackGround);
PHP_METHOD(ExcelCellFormat, setFont);
PHP_METHOD(ExcelCellFormat, setAlignment);

#endif //End _HAVE_PHPEXCEL_FORMAT_H__
