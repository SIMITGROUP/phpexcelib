#ifndef GBKCHARSETCONVERTOR_H
#define GBKCHARSETCONVERTOR_H

#include "CharsetConvertor.h"
#ifndef PHP_WIN32
#include <iconv.h>
#endif

class GBKCharsetConvertor : public ICharsetConvertor
{
public:
#ifndef PHP_WIN32
    GBKCharsetConvertor();
    ~GBKCharsetConvertor();
#endif
    wchar_t*    stwCvt(const char* str, size_t len);
    char*       wtsCvt(const wchar_t* wstr, size_t len);

#ifndef PHP_WIN32
private:
    iconv_t m_ivStw;
    iconv_t m_ivWts;
#endif
};

#endif // GBKCHARSETCONVERTOR_H
