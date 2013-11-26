#include "PHPExcel.hpp"
#include "PHPExcelSheet.hpp"
#include "Util.hpp"
#include "php_excel.h"

extern "C" {
#include "zend_exceptions.h"
}

extern	zend_class_entry*		excel_ce;
extern	zend_class_entry*		excel_sheet_ce;
extern	zend_object_handlers	excel_object_handlers;

#ifdef  ZTS
extern  int                 excel_globals_id;
#else
extern  zend_excel_globals  excel_globals;
#endif

zend_function_entry	excel_methods[]	= {
	PHP_ME(Excel, __construct, NULL, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
	PHP_ME(Excel, open, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
	PHP_ME(Excel, create, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Excel, getCharset, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Excel, getWorkSheet, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Excel, getTotalWorkSheets, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Excel, saveAs, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

void	excel_free_storage(void* object TSRMLS_DC)
{
	excel_object*	obj	= (excel_object*) object;
	if (obj->pExcel != NULL)
	{
		delete	obj->pExcel;
	}
	if (obj->pFmtMgr != NULL)
	{
		delete	obj->pFmtMgr;
	}
    if (obj->szCharset != EXCEL_G(default_charset)
            && obj->szCharset != NULL)
    {
        efree(obj->szCharset);
    }
	zend_object_std_dtor(&obj->std TSRMLS_CC);

	efree(obj);
}

zend_object_value	excel_create_handler(zend_class_entry* type TSRMLS_DC)
{
	zend_object_value	retVal;

	excel_object*	obj	= (excel_object*) emalloc(sizeof(excel_object));
	zend_object_std_init(&obj->std, type TSRMLS_CC);

#if PHP_VERSION_ID < 50399
	zend_hash_copy(obj->std.properties, &(type->default_properties), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));
#else 
	object_properties_init(&obj->std, type);
#endif
	retVal.handle	= zend_objects_store_put(obj, NULL, excel_free_storage, NULL TSRMLS_CC);
	retVal.handlers	= &excel_object_handlers;

	return retVal;
}

void register_excel_class_ce(TSRMLS_D)
{
	zend_class_entry	ce;
	memcpy(&excel_object_handlers, zend_get_std_object_handlers(), sizeof(excel_object_handlers));
	excel_object_handlers.clone_obj	= NULL;

	INIT_CLASS_ENTRY(ce, "Excel", excel_methods);
	excel_ce	= zend_register_internal_class(&ce TSRMLS_CC);
	excel_ce->create_object	= excel_create_handler;
}

PHP_METHOD(Excel, __construct)
{
}

PHP_METHOD(Excel, getWorkSheet)
{
	int	nSheetIndex	= 0;
	BasicExcelWorksheet* pSheet			= NULL;
	excel_sheet_object*	 pSheetObject	= NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nSheetIndex) == FAILURE)
	{
		RETURN_FALSE;
	}
	excel_object*	obj	= (excel_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
	if (obj->pExcel)
	{
		pSheet	= obj->pExcel->GetWorksheet(nSheetIndex);
	}
	if (pSheet == NULL)
	{
        php_error(E_WARNING, "Cann't Found Sheet, total %d sheets, query %d sheet", obj->pExcel->GetTotalWorkSheets(), nSheetIndex);
		RETURN_FALSE;
	}
	        php_error(E_WARNING, "warning 1");
	if (object_init_ex(return_value, excel_sheet_ce) != SUCCESS)
	{
		RETURN_FALSE;
    }
    	        php_error(E_WARNING, "warning 2");
	pSheetObject	= (excel_sheet_object*) zend_object_store_get_object(return_value TSRMLS_CC);
	if (pSheetObject != NULL)
	{
        pSheetObject->pExcelSheet           = pSheet;
        pSheetObject->pCharsetConvertor     = getCharsetConverotr(obj->szCharset);
		return;
	}
		        php_error(E_WARNING, "warning 3");
	RETURN_FALSE;
}

PHP_METHOD(Excel, saveAs)
{
	char*	szSavePath	= NULL;
	int		nSavePathLen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &szSavePath, &nSavePathLen) == FAILURE)
	{
		RETURN_FALSE;
	}
	excel_object*	obj	= (excel_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
	if (obj->pExcel != NULL)
	{
		if (YSUCCESS == obj->pExcel->SaveAs(szSavePath))
		{
			RETURN_TRUE;
		}
	}
	RETURN_FALSE;
}

PHP_METHOD(Excel, open)
{
	char*	szPath;
	int		nPathLen;
	bool	isError	= false;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &szPath, &nPathLen) == FAILURE)
	{
		RETURN_FALSE;
	}

	BasicExcel*	excel	= new BasicExcel();
	if (!excel->Load(szPath))
	{
		isError	= true;
	} else if (object_init_ex(return_value, excel_ce) != SUCCESS)
	{
		isError	= true;
	}
	if (isError)
	{
		delete	excel;

		RETURN_FALSE;
	}
	excel_object*	obj	= (excel_object*) zend_object_store_get_object(return_value TSRMLS_CC);
	obj->pExcel			= excel;
	obj->pFmtMgr		= NULL;
}

PHP_METHOD(Excel, getTotalWorkSheets)
{
	excel_object*	obj	= (excel_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
	if (obj->pExcel)
	{
		RETURN_LONG(obj->pExcel->GetTotalWorkSheets());
	}
	RETURN_FALSE;
}

PHP_METHOD(Excel, getCharset)
{
    excel_object*	obj	= (excel_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj->szCharset != NULL)
    {
        RETURN_STRING(obj->szCharset, 1);
    } else {
        RETURN_STRING(EXCEL_G(default_charset), 1);
    }
}

PHP_METHOD(Excel, create)
{
    int	nStrLen, nSheetNum		= 1;
    char*   szCharset   = NULL;
	BasicExcel*	excel	= NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|ls", &nSheetNum, &szCharset, &nStrLen) == FAILURE)
	{
		RETURN_FALSE;
	}
	if (nSheetNum < 1)
	{
		nSheetNum	= 1;
	}
	if (object_init_ex(return_value, excel_ce) != SUCCESS)
	{
		RETURN_FALSE;
	}
	excel	= new BasicExcel();
	excel->New(nSheetNum);

	excel_object*	obj	= (excel_object*) zend_object_store_get_object(return_value TSRMLS_CC);
	obj->pExcel		= excel;
	obj->pFmtMgr	= NULL;
    if (szCharset != NULL)
    {
        char*   charset = (char*) emalloc(sizeof(char) * (nStrLen + 1));
        strncpy(charset, szCharset, nStrLen + 1);
        obj->szCharset      = charset;
    } else {
        obj->szCharset      = EXCEL_G(default_charset);
    }
}
