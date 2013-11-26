#ifndef	_HAVE_PHPEXCEL_SHEET_H__
#define	_HAVE_PHPEXCEL_SHEET_H__

#define PHP_EXCEL_SETVALUE(row, col, val, setFuncName, fmt)                         \
    excel_sheet_object* obj =                                                       \
        (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);     \
    if (obj != NULL)                                                                \
    {                                                                               \
        BasicExcelCell* pCell   = obj->pExcelSheet->Cell(nRow, nCol);               \
        if (pCell != NULL)                                                          \
        {                                                                           \
            pCell->setFuncName(val);                                                \
            PHP_EXCEL_SET_FORMAT(pCell, fmt)                                        \
            RETURN_TRUE;                                                            \
        } else {                                                                    \
            php_error(E_WARNING, "row %d col %d Cann't Find Cell Object");          \
        }                                                                           \
    } else {                                                                        \
        php_error(E_WARNING, "Object Cann't obtain underlaying ExcelObject");       \
    }                                                                               \
    RETURN_FALSE;

#define PHP_EXCEL_SET_FORMAT(pCell, fmt)                                            \
    if (fmt != NULL)                                                                \
    {                                                                               \
        cell_format_object*	fmtObj	=                                               \
            (cell_format_object*) zend_object_store_get_object(zvalFmt TSRMLS_CC);  \
        if (fmtObj != NULL && fmtObj->pCellFormat != NULL)                          \
        {                                                                           \
            pCell->SetFormat(* fmtObj->pCellFormat);                                \
        } else {                                                                    \
            php_error(E_WARNING, "Argument Cell Format is Not a Valid Cell Format");\
        }                                                                           \
    }

extern	"C" {
#include "php.h"
}

#include "ExcelFormat.hpp"
#include "Util.hpp"
#include "CharsetConvertor.h"

using namespace ExcelFormat;

struct excel_sheet_object {
	zend_object				std;
	BasicExcelWorksheet*	pExcelSheet;
    ICharsetConvertor*      pCharsetConvertor;
};

void	excel_sheet_free_storage(void* object TSRMLS_DC);
zend_object_value	excel_sheet_create_handler(zend_class_entry* type TSRMLS_DC);
void	register_excel_sheet_class_ce(TSRMLS_D);

PHP_METHOD(ExcelSheet, __construct);
PHP_METHOD(ExcelSheet, setInteger);
PHP_METHOD(ExcelSheet, setDouble);
PHP_METHOD(ExcelSheet, setAnsiString);
PHP_METHOD(ExcelSheet, setWString);
PHP_METHOD(ExcelSheet, setFormat);
PHP_METHOD(ExcelSheet, setColWidth);
PHP_METHOD(ExcelSheet, mergeCells);
PHP_METHOD(ExcelSheet, getTotalRows);
PHP_METHOD(ExcelSheet, getTotalCols);
PHP_METHOD(ExcelSheet, eraseCell);
PHP_METHOD(ExcelSheet, getColWidth);
PHP_METHOD(ExcelSheet, getSheetName);
PHP_METHOD(ExcelSheet, getValue);

#endif
