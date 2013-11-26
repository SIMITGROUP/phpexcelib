#include "utf8charsetconvertor.h"
#include "Util.hpp"

#ifndef PHP_WIN32
UTF8CharsetConvertor::UTF8CharsetConvertor()
{
    m_ivStw  = iconv_open("WCHAR_T", "UTF-8");
    m_ivWts  = iconv_open("UTF-8", "WCHAR_T");
}

UTF8CharsetConvertor::~UTF8CharsetConvertor()
{
    if (m_ivStw != (iconv_t) - 1)
    {
        iconv_close(m_ivStw);
    }
    if (m_ivWts != (iconv_t) - 1)
    {
        iconv_close(m_ivWts);
    }
}
#endif

wchar_t* UTF8CharsetConvertor::stwCvt(const char *str, size_t len)
{
#ifdef  PHP_WIN32
    DWORD dwLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    wchar_t* buffer = (wchar_t*) emalloc(sizeof(wchar_t) * dwLen);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer, dwLen);
    return buffer;
#else
    wchar_t*  buffer;
    size_t    outLen;
    iconv_convert_string(m_ivStw, str, len, (char**)&buffer, &outLen);
    return buffer;
#endif
}

char*  UTF8CharsetConvertor::wtsCvt(const wchar_t *wstr, size_t len)
{
#ifdef  PHP_WIN32
    DWORD  dwLen    = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char*   buffer  = (char*) emalloc(sizeof(char) * dwLen);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, dwLen, NULL, NULL);
    return buffer;
#else
    char*   buffer;
    size_t  outLen;
    iconv_convert_string(m_ivWts, (const char*) wstr, sizeof(wchar_t) * len, &buffer, &outLen);
    return buffer;
#endif
}
