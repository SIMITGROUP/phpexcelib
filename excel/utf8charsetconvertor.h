#ifndef UTF8CHARSETCONVERTOR_H
#define UTF8CHARSETCONVERTOR_H

#include "CharsetConvertor.h"
#ifndef PHP_WIN32
#include <iconv.h>
#endif

class UTF8CharsetConvertor : public ICharsetConvertor
{
public:
#ifndef PHP_WIN32
    UTF8CharsetConvertor();
    ~UTF8CharsetConvertor();
#endif
    wchar_t*    stwCvt(const char* str, size_t len);
    char*       wtsCvt(const wchar_t* wstr, size_t len);

#ifndef PHP_WIN32
private:
    iconv_t m_ivStw;
    iconv_t m_ivWts;
#endif
};

#endif // UTF8CHARSETCONVERTOR_H
