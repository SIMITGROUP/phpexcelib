#ifndef CHARSETCONVERTOR_H
#define CHARSETCONVERTOR_H

#ifdef PHP_WIN32
#define     WIN32_LEAN_AND_MEAN
#include    <Windows.h>
#endif

extern "C" {
#include "php.h"
}

class ICharsetConvertor
{
public:
    virtual char*       wtsCvt(const wchar_t* wstr, size_t len) = 0;
    virtual wchar_t*    stwCvt(const char* str, size_t len)     = 0;
    virtual ~ICharsetConvertor()
    {
    }

    void*  operator new(size_t size)
    {
        return emalloc(size);
    }

    void  operator delete(void* ptr)
    {
        efree(ptr);
    }
};

#endif // CHARSETCONVERTOR_H
