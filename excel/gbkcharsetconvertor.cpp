#include "gbkcharsetconvertor.h"
#include "Util.hpp"

#ifndef PHP_WIN32
GBKCharsetConvertor::GBKCharsetConvertor()
{
    m_ivStw = iconv_open("WCHAR_T", "GBK");
    m_ivWts = iconv_open("GBK", "WCHAR_T");
}

GBKCharsetConvertor::~GBKCharsetConvertor()
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

wchar_t* GBKCharsetConvertor::stwCvt(const char *str, size_t len)
{
#ifdef  PHP_WIN32
    DWORD dwLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    wchar_t* buffer = (wchar_t*) emalloc(sizeof(wchar_t) * dwLen);
    MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, dwLen);
    return buffer;
#else
    wchar_t*  buffer;
    size_t    outLen;
    iconv_convert_string(m_ivStw, str, len, (char**)&buffer, &outLen);
    return buffer;
#endif
}

char*  GBKCharsetConvertor::wtsCvt(const wchar_t *wstr, size_t len)
{
#ifdef  PHP_WIN32
    DWORD  dwLen    = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char*   buffer  = (char*) emalloc(sizeof(char) * dwLen);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, buffer, dwLen, NULL, NULL);
    return buffer;
#else
    char*   buffer;
    size_t  outLen;
    iconv_convert_string(m_ivWts, (const char*) wstr, sizeof(wchar_t) * len, &buffer, &outLen);
    return buffer;
#endif
}
