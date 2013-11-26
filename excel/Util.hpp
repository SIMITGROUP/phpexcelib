#ifndef UTIL_HPP
#define UTIL_HPP

#include "CharsetConvertor.h"
#ifndef PHP_WIN32
#include <iconv.h>
#define strnicmp    strncasecmp
#endif

ICharsetConvertor*  getCharsetConverotr(const char* charset);

#ifdef  PHP_WIN32
char*   strncpy(char* dest, char* src, size_t n);
#else
bool iconv_convert_string(iconv_t iv, const char* in_p, size_t in_len, char** out, size_t* out_len);
#endif

#endif // UTIL_HPP
