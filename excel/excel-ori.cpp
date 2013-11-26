/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_excel.h"
}

#include "PHPExcel.hpp"
#include "PHPExcelFont.hpp"
#include "PHPExcelFormat.hpp"
#include "PHPExcelSheet.hpp"

zend_object_handlers	excel_object_handlers;
zend_object_handlers	cell_format_object_handlers;
zend_object_handlers	excel_font_object_handlers;
zend_object_handlers	excel_sheet_object_handlers;

zend_class_entry*	excel_ce;
zend_class_entry*	cell_format_ce;
zend_class_entry*	excel_font_ce;
zend_class_entry*	excel_sheet_ce;

/* If you declare any globals in php_excel.h uncomment this:*/
ZEND_DECLARE_MODULE_GLOBALS(excel)

/* True global resources - no need for thread safety here */
static int le_excel;

/* {{{ excel_functions[]
 *
 * Every user visible function must have an entry in excel_functions[].
 */
const zend_function_entry excel_functions[] = {
	PHP_FE(confirm_excel_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in excel_functions[] */
};
/* }}} */

/* {{{ php_excel_init_globals
 */
/* Uncomment this function if you have INI entries */
PHP_GINIT_FUNCTION(excel)
{
    excel_globals->default_charset  = "UTF-8";
}

/* {{{ excel_module_entry
 */
zend_module_entry excel_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"excel",
	excel_functions,
	PHP_MINIT(excel),
	PHP_MSHUTDOWN(excel),
    NULL,		/* Replace with NULL if there's nothing to do at request start */
    NULL,	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(excel),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
    PHP_MODULE_GLOBALS(excel),
    PHP_GINIT(excel),
    NULL,
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_EXCEL
ZEND_GET_MODULE(excel)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini*/
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("excel.default_charset",      "UTF-8", PHP_INI_ALL, OnUpdateStringUnempty, default_charset, zend_excel_globals, excel_globals)
PHP_INI_END()
/* }}} */

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(excel)
{
	REGISTER_INI_ENTRIES();

    register_excel_class_ce(TSRMLS_C);
    register_excel_sheet_class_ce(TSRMLS_C);
    register_excel_font_class_ce(TSRMLS_C);
    register_cell_format_class_ce(TSRMLS_C);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(excel)
{
	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(excel)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(excel)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(excel)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "excel support", "enabled");
    php_info_print_table_row(2, "author", "zhangsilly@gmail.com");
    php_info_print_table_row(2, "version", "0.1");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_excel_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_excel_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "excel", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
