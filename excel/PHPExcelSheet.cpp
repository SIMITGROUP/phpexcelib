#include "PHPExcelSheet.hpp"
#include "PHPExcelFormat.hpp"

extern	zend_object_handlers	excel_sheet_object_handlers;
extern	zend_class_entry*		excel_sheet_ce;
extern	zend_class_entry*		cell_format_ce;

ZEND_BEGIN_ARG_INFO(ExcelSheet_SetValue_ARGINFO, 0)
	ZEND_ARG_INFO(0, row)
	ZEND_ARG_INFO(0, col)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_OBJ_INFO(1, format, ExcelCellFormat, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(ExcelSheet_SetFormat_ARGINFO, 0)
	ZEND_ARG_INFO(0, row)
	ZEND_ARG_INFO(0, col)
	ZEND_ARG_OBJ_INFO(1, format, ExcelCellFormat, 0)
ZEND_END_ARG_INFO()

zend_function_entry	excel_sheet_methods[] = {
	PHP_ME(ExcelSheet, __construct, NULL, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	PHP_ME(ExcelSheet, setInteger, ExcelSheet_SetValue_ARGINFO, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, setDouble, ExcelSheet_SetValue_ARGINFO, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, setAnsiString, ExcelSheet_SetValue_ARGINFO, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, setWString, ExcelSheet_SetValue_ARGINFO, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, setFormat, ExcelSheet_SetFormat_ARGINFO, ZEND_ACC_PUBLIC)

	PHP_ME(ExcelSheet, setColWidth, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, mergeCells, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, getTotalRows, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, eraseCell, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, getColWidth, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelSheet, getSheetName, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(ExcelSheet, getValue, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void excel_sheet_free_storage(void* object TSRMLS_DC)
{
	excel_sheet_object*	obj	= (excel_sheet_object*) object;
	if (obj->pExcelSheet == NULL)
	{
		delete	obj->pExcelSheet;
	}
    if (obj->pCharsetConvertor != NULL)
    {
        delete obj->pCharsetConvertor;
    }
	zend_object_std_dtor(&obj->std TSRMLS_CC);
	efree(obj);
}

zend_object_value	excel_sheet_create_handler(zend_class_entry* type TSRMLS_DC)
{
	zend_object_value	retVal;

	excel_sheet_object*	obj	= (excel_sheet_object*) emalloc(sizeof(excel_sheet_object));
	zend_object_std_init(&obj->std, type TSRMLS_CC);

#if PHP_VERSION_ID < 50399
	zend_hash_copy(obj->std.properties, &(type->default_properties), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));
#else 
	object_properties_init(&obj->std, type);
#endif
	retVal.handle	= zend_objects_store_put(obj, NULL, excel_sheet_free_storage, NULL TSRMLS_CC);
	retVal.handlers	= &excel_sheet_object_handlers;

	return retVal;
}

void register_excel_sheet_class_ce(TSRMLS_D)
{
	zend_class_entry	ce;
	memcpy(&excel_sheet_object_handlers, zend_get_std_object_handlers(), sizeof(excel_sheet_object_handlers));
	excel_sheet_object_handlers.clone_obj	= NULL;

	INIT_CLASS_ENTRY(ce, "ExcelSheet", excel_sheet_methods);
	excel_sheet_ce	= zend_register_internal_class(&ce TSRMLS_CC);
	excel_sheet_ce->create_object	= excel_sheet_create_handler;
}

PHP_METHOD(ExcelSheet, __construct)
{

}

PHP_METHOD(ExcelSheet, setInteger)
{
	long	nRow, nCol, nVal;
	zval* zvalFmt	= NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll|O",  &nCol,&nRow, &nVal, &zvalFmt, cell_format_ce) == FAILURE)
	{
		RETURN_FALSE;
	}
    PHP_EXCEL_SETVALUE(nRow, nCol, nVal, SetInteger, zvalFmt)
}

PHP_METHOD(ExcelSheet, setDouble)
{
    long nRow, nCol;
    double  dValue;
    zval*   zvalFmt = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lld|O",  &nCol,&nRow, &dValue, &zvalFmt, cell_format_ce) == FAILURE)
    {
        RETURN_FALSE;
    }
    PHP_EXCEL_SETVALUE(nRow, nCol, dValue, SetDouble, zvalFmt)
}

PHP_METHOD(ExcelSheet, setAnsiString)
{
    long nRow, nCol, nStrLen;
    char*  szStr;
    zval*  zvalFmt = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls|O", &nCol, &nRow, &szStr, &nStrLen, &zvalFmt, cell_format_ce) == FAILURE)
    {
        RETURN_FALSE;
    }
    PHP_EXCEL_SETVALUE(nRow, nCol, szStr, SetString, zvalFmt)
}

PHP_METHOD(ExcelSheet, setWString)
{
    int nRow, nCol, nStrLen;
    char*   szStr;
    zval*   zvalFmt = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lls|O", &nCol, &nRow, &szStr, &nStrLen, &zvalFmt, cell_format_ce) == FAILURE)
    {
        RETURN_FALSE;
    }
    excel_sheet_object* obj =
            (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL)
    {
        BasicExcelCell* pCell   = obj->pExcelSheet->Cell(nRow, nCol);
        if (pCell != NULL)
        {
            wchar_t*  buffer    = obj->pCharsetConvertor->stwCvt(szStr, nStrLen);
        
            if (buffer != NULL)
            {

                               
                                            pCell->SetWString(buffer);
			efree(buffer);              
            //  pCell->SetString(szStr);

                PHP_EXCEL_SET_FORMAT(pCell, zvalFmt)

                RETURN_TRUE;
            }
        }
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelSheet, setFormat)
{
    long nRow, nCol;
    zval*  zvalFmt  = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llO", &nCol,&nRow,  &zvalFmt, cell_format_ce) == FAILURE)
    {
        RETURN_FALSE;
    }
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        BasicExcelCell* pCell   = obj->pExcelSheet->Cell(nRow, nCol);
        if (pCell != NULL)
        {
            cell_format_object* fmtObj  = (cell_format_object*) zend_object_store_get_object(zvalFmt TSRMLS_CC);
            if (fmtObj != NULL && fmtObj->pCellFormat != NULL)
            {
                pCell->SetFormat(* fmtObj->pCellFormat);
                RETURN_TRUE;
            } else {
                php_error(E_WARNING, "Argument Cell Format is Not a valid Cell Format");
            }
        }
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelSheet, setColWidth)
{

    long nColIndex;
    long nColWidth;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nColIndex, &nColWidth) == FAILURE)
    {
        RETURN_FALSE;
    }
    
//    php_printf("setcolwidth arg: %l and %l <br/>",nColIndex, nColWidth);
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        obj->pExcelSheet->SetColWidth(nColIndex, nColWidth);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelSheet, getColWidth)
{
    long nColIndex;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nColIndex) == FAILURE)
    {
        RETURN_FALSE;
    }
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        RETURN_LONG(obj->pExcelSheet->GetColWidth(nColIndex));
    }
    RETURN_LONG(0L);
}

PHP_METHOD(ExcelSheet, mergeCells)
{
    long nRow, nCol, nRangeRow, nRangeCol;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &nCol, &nRow, &nRangeCol, &nRangeRow) == FAILURE)
    {
        RETURN_FALSE;
    }
    
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet)
    {
        obj->pExcelSheet->MergeCells( nRow, nCol, nRangeRow, nRangeCol);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelSheet, getSheetName)
{
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        char* szSheetName   = obj->pExcelSheet->GetAnsiSheetName();
        if (szSheetName == NULL)
        {
            wchar_t*  wSheetName    = obj->pExcelSheet->GetUnicodeSheetName();
            size_t  len = -1;
#ifndef PHP_WIN32
            len = wcslen(wSheetName);
#endif
            RETURN_STRING(obj->pCharsetConvertor->wtsCvt(wSheetName, len), 0);
        } else {
            RETURN_STRING(szSheetName, 1);
        }
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelSheet, getTotalRows)
{
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        RETURN_LONG(obj->pExcelSheet->GetTotalRows());
    }
    RETURN_LONG(0);
}

PHP_METHOD(ExcelSheet, getTotalCols)
{
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        RETURN_LONG(obj->pExcelSheet->GetTotalCols());
    }
    RETURN_LONG(0);
}

PHP_METHOD(ExcelSheet, eraseCell)
{
    long nRow, nCol;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll",  &nCol,&nRow) == FAILURE)
    {
        RETURN_FALSE;
    }
    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        obj->pExcelSheet->EraseCell(nRow, nCol);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelSheet, getValue)
{
    long nRow, nCol;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &nCol, &nRow) == FAILURE)
    {
        RETURN_FALSE;
    }

    excel_sheet_object* obj = (excel_sheet_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL && obj->pExcelSheet != NULL)
    {
        BasicExcelCell* pCell   = obj->pExcelSheet->Cell(nRow, nCol);
        if (pCell != NULL)
        {
            switch (pCell->Type())
            {
            case BasicExcelCell::INT:
                RETURN_LONG(pCell->GetInteger());
            case BasicExcelCell::DOUBLE:
                RETURN_DOUBLE(pCell->GetDouble());
            case BasicExcelCell::STRING:
                RETURN_STRINGL(pCell->GetString(), pCell->GetStringLength(), 1);
            case BasicExcelCell::WSTRING:
                {
                    const wchar_t* wval   = pCell->GetWString();
                    size_t  len     = -1;
#ifndef PHP_WIN32
                    len = wcslen(wval);
#endif
                    char* value = obj->pCharsetConvertor->wtsCvt(wval, len);
                    RETURN_STRING(value, 0);
                }
            default:
                RETURN_STRING("UnSupported Cell Type", 1);
            }
        }
    }
    RETURN_FALSE;
}
