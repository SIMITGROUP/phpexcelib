#include "PHPExcelFormat.hpp"
#include "PHPExcel.hpp"
#include "PHPExcelFont.hpp"

#define FMT_CONST(const_name, const_value, const_type)          \
    zend_declare_class_constant_##const_type(cell_format_ce,    \
        #const_name,                                            \
        sizeof(#const_name) - 1,                                \
        const_value TSRMLS_CC)

extern zend_object_handlers	cell_format_object_handlers;
extern zend_class_entry*	cell_format_ce;
extern zend_class_entry*    excel_ce;
extern zend_class_entry*    excel_font_ce;

zend_function_entry	cell_format_methods[]	= {
    PHP_ME(ExcelCellFormat, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(ExcelCellFormat, setBackGround, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelCellFormat, setFont, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelCellFormat, setAlignment, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

void	cell_format_free_storage(void* object TSRMLS_DC)
{
    cell_format_object* obj = (cell_format_object*) object;
    if (obj->pCellFormat != NULL)
    {
        delete  obj->pCellFormat;
    }
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value	cell_format_create_handler(zend_class_entry* type TSRMLS_DC)
{
    zend_object_value   retVal;

    cell_format_object* obj = (cell_format_object*) emalloc(sizeof(cell_format_object));
    zend_object_std_init(&obj->std, type TSRMLS_CC);

#if PHP_VERSION_ID < 50399
    zend_hash_copy(obj->std.properties, &(type->default_properties), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));
#else
    object_properties_init(&obj->std, type);
#endif
    retVal.handle   = zend_objects_store_put(obj, NULL, cell_format_free_storage, NULL TSRMLS_CC);
    retVal.handlers = &cell_format_object_handlers;

    return retVal;
}

void	register_cell_format_class_ce(TSRMLS_D)
{
    zend_class_entry    ce;
    memcpy(&cell_format_object_handlers, zend_get_std_object_handlers(), sizeof(cell_format_object_handlers));
    cell_format_object_handlers.clone_obj   = NULL;

    INIT_CLASS_ENTRY(ce, "ExcelCellFormat", cell_format_methods);
    cell_format_ce  = zend_register_internal_class(&ce TSRMLS_CC);
    cell_format_ce->create_object   = cell_format_create_handler;

    FMT_CONST(HALIGN_GENERAL, EXCEL_HALIGN_GENERAL, long);
    FMT_CONST(HALIGN_LEFT, EXCEL_HALIGN_LEFT, long);
    FMT_CONST(HALIGN_CENTRED, EXCEL_HALIGN_CENTRED, long);
    FMT_CONST(HALIGN_RIGHT, EXCEL_HALIGN_RIGHT, long);
    FMT_CONST(HALIGN_FILLED, EXCEL_HALIGN_FILLED, long);
    FMT_CONST(HALIGN_JUSTIFIED, EXCEL_HALIGN_JUSITFIED, long);
    FMT_CONST(HALIGN_SEL_CENTRED, EXCEL_HALIGN_SEL_CENTRED, long);
    FMT_CONST(HALIGN_DISTRIBUTED, EXCEL_HALIGN_DISTRIBUTED, long);
    FMT_CONST(VALIGN_TOP, EXCEL_VALIGN_TOP, long);
    FMT_CONST(VALIGN_CENTRED, EXCEL_VALIGN_CENTRED, long);
    FMT_CONST(VALIGN_BOTTOM, EXCEL_VALIGN_BOTTOM, long);
    FMT_CONST(VALIGN_JUSTIFIED, EXCEL_VALIGN_JUSTIFIED, long);
    FMT_CONST(JUSTIFY_LAST_LINE, EXCEL_JUSTIFY_LAST_LINE, long);
}

PHP_METHOD(ExcelCellFormat, __construct)
{
    zval*    zvalExcel  = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zvalExcel, excel_ce) == FAILURE)
    {
        RETURN_NULL();
    }

    cell_format_object* obj     = (cell_format_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    excel_object*   excelObj    = (excel_object*) zend_object_store_get_object(zvalExcel TSRMLS_CC);
    if (excelObj->pFmtMgr == NULL)
    {
        excelObj->pFmtMgr   = new XLSFormatManager(*excelObj->pExcel);
    }
    obj->pCellFormat    = new CellFormat(*excelObj->pFmtMgr);
}

PHP_METHOD(ExcelCellFormat, setBackGround)
{
    long nColor;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nColor) == FAILURE)
    {
        RETURN_FALSE;
    }
    if (nColor < EGA_BLACK || nColor > EGA_CYAN)
    {
        RETURN_FALSE;
    }
    cell_format_object* fmtObj  = (cell_format_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (fmtObj != NULL && fmtObj->pCellFormat != NULL)
    {
	    fmtObj->pCellFormat->set_background(MAKE_COLOR2(nColor,0));
	  
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelCellFormat, setFont)
{
    zval*   zvalFont;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &zvalFont, excel_font_ce) == FAILURE)
    {
        RETURN_FALSE;
    }
    cell_format_object* fmtObj  = (cell_format_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    excel_font_object*  fntObj  = (excel_font_object*) zend_object_store_get_object(zvalFont TSRMLS_CC);
    if (fmtObj != NULL
            && fmtObj->pCellFormat != NULL
            && fntObj != NULL
            && fntObj->pExcelFont != NULL)
    {
        fmtObj->pCellFormat->set_font(* fntObj->pExcelFont);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_METHOD(ExcelCellFormat, setAlignment)
{
    long nAlignment;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nAlignment) == FAILURE)
    {
        RETURN_FALSE;
    }

    cell_format_object* fmtObj  = (cell_format_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (fmtObj != NULL && fmtObj->pCellFormat != NULL)
    {
        fmtObj->pCellFormat->set_alignment((char) nAlignment);
        RETURN_TRUE;
    }
    RETURN_FALSE;
}
