#include "PHPExcelFont.hpp"
#include "utf8charsetconvertor.h"

#define FNT_CONST(const_name, const_value, const_type)          \
    zend_declare_class_constant_##const_type(excel_font_ce,     \
        #const_name,                                            \
        sizeof(#const_name) - 1,                                \
        const_value TSRMLS_CC)

#define FNT_PROPERTY_SET(set_function_name, set_value)                          \
    excel_font_object*  fntObj  =                                               \
        (excel_font_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);  \
    if (fntObj != NULL && fntObj->pExcelFont != NULL)                           \
    {                                                                           \
        fntObj->pExcelFont->set_function_name(set_value);                       \
        RETURN_TRUE;                                                            \
    }                                                                           \
    RETURN_FALSE;

extern zend_object_handlers excel_font_object_handlers;
extern zend_class_entry*    excel_font_ce;

zend_function_entry	excel_font_methods[]	= {
    PHP_ME(ExcelFont, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(ExcelFont, setWeight, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelFont, setFontSize, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelFont, setColor, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelFont, setItalic, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelFont, setFontName, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(ExcelFont, setUnderline, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

void   excel_font_free_storage(void* object TSRMLS_DC)
{
    excel_font_object*  obj = (excel_font_object*) object;
    if (obj->pExcelFont != NULL)
    {
        delete  obj->pExcelFont;
    }
    zend_object_std_dtor(&obj->std TSRMLS_CC);
    efree(obj);
}

zend_object_value   excel_font_create_handler(zend_class_entry* type TSRMLS_DC)
{
    zend_object_value   retVal;

    excel_font_object*  obj = (excel_font_object*) emalloc(sizeof(excel_font_object));
    zend_object_std_init(&obj->std, type TSRMLS_CC);

#if PHP_VERSION_ID < 50399
    zend_hash_copy(obj->std.properties, &(type->default_properties), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*));
#else
    object_properties_init(&obj->std, type);
#endif
    retVal.handle   = zend_objects_store_put(obj, NULL, excel_font_free_storage, NULL TSRMLS_CC);
    retVal.handlers = &excel_font_object_handlers;

    return retVal;
}

void    register_excel_font_class_ce(TSRMLS_D)
{
    zend_class_entry    ce;
    memcpy(&excel_font_object_handlers, zend_get_std_object_handlers(), sizeof(excel_font_object_handlers));
    excel_font_object_handlers.clone_obj    = NULL;

    INIT_CLASS_ENTRY(ce, "ExcelFont", excel_font_methods);
    excel_font_ce   = zend_register_internal_class(&ce TSRMLS_CC);
    excel_font_ce->create_object    = excel_font_create_handler;

    FNT_CONST(WEIGHT_NORMAL, 400, long);
    FNT_CONST(WEIGHT_BOLD, 700, long);
    FNT_CONST(COLOR_BLACK, EGA_BLACK, long);
    FNT_CONST(COLOR_WHITE, EGA_WHITE, long);
    FNT_CONST(COLOR_RED, EGA_RED, long);
    FNT_CONST(COLOR_GREEN, EGA_GREEN, long);
    FNT_CONST(COLOR_YELLOW, EGA_YELLOW, long);
    FNT_CONST(COLOR_MAGENTA, EGA_MAGENTA, long);
    FNT_CONST(COLOR_CYAN, EGA_CYAN, long);
}

PHP_METHOD(ExcelFont, __construct)
{
    int nFontWeight = 400;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &nFontWeight) == FAILURE)
    {
        RETURN_NULL();
    }

    excel_font_object*  obj = (excel_font_object*) zend_object_store_get_object(this_ptr TSRMLS_CC);
    if (obj != NULL)
    {
        obj->pExcelFont = new ExcelFont();
        obj->pExcelFont->set_weight(nFontWeight);
    }
}

PHP_METHOD(ExcelFont, setWeight)
{
    int nFontWeight = 400;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nFontWeight) == FAILURE)
    {
        RETURN_NULL();
    }
    FNT_PROPERTY_SET(set_weight, nFontWeight)
}
PHP_METHOD(ExcelFont, setFontSize)
{
    int nMultiply = 20;  // 200 = > 10
	long nSize;
	long size;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nSize) == FAILURE)
    {
        RETURN_NULL();
    }
    
    size=nSize*nMultiply;
    FNT_PROPERTY_SET(set_height, size)
}

PHP_METHOD(ExcelFont, setColor)
{
    int nColorIndex = EGA_BLACK;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &nColorIndex) == FAILURE)
    {
        RETURN_FALSE;
    }
    if (nColorIndex < EGA_BLACK || nColorIndex > EGA_CYAN)
    {
        RETURN_FALSE;
    }
    FNT_PROPERTY_SET(set_color_index, nColorIndex)
}

PHP_METHOD(ExcelFont, setItalic)
{
    zend_bool   bIsItalic;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &bIsItalic) == FAILURE)
    {
        RETURN_FALSE;
    }
    FNT_PROPERTY_SET(set_italic, bIsItalic)
}

PHP_METHOD(ExcelFont, setUnderline)
{
    zend_bool bIsUnderLine;
	char underlinetype=0x01;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &bIsUnderLine) == FAILURE)
    {
        RETURN_FALSE;
    }
//        FNT_PROPERTY_SET(set_underline_type, 0x01)

    if(bIsUnderLine==true)
    {
   		FNT_PROPERTY_SET(set_underline_type, 0x01)
   		}
    else{
        FNT_PROPERTY_SET(set_underline_type, 0x00)
        }
}


PHP_METHOD(ExcelFont, setFontName)
{
     char*  szStr;
     int nStrLen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &szStr, &nStrLen) == FAILURE)
    {
        RETURN_FALSE;
    }

//ICharsetConvertor * fobj = new UTF8CharsetConvertor();
    //    RETURN_FALSE;
  // wchar_t * ft= fobj->stwCvt(szStr, nStrLen);
    //    FNT_PROPERTY_SET(set_font_name, ft);    
}
