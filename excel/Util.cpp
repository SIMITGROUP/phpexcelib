#include "Util.hpp"
#include "utf8charsetconvertor.h"
#include "gbkcharsetconvertor.h"

extern "C" {
#include "php.h"
}

ICharsetConvertor*  getCharsetConverotr(const char* charset)
{
    if (strnicmp("UTF-8", charset, sizeof("UTF-8") - 1) == 0
            || strnicmp("UTF8", charset, sizeof("UTF8") - 1) == 0)
    {
        return new UTF8CharsetConvertor();
    } else {
        return new GBKCharsetConvertor();
    }
}

#ifdef  PHP_WIN32

char*   strncpy(char* dest, char* src, size_t n)
{
    errno_t ret = strncpy_s(dest, n, src, _TRUNCATE);
    if (ret != EINVAL)
    {
        return dest;
    }
    return NULL;
}

#else

bool iconv_convert_string(iconv_t iv, const char* in_p, size_t in_len, char** out, size_t* out_len)
{
    size_t  in_left, out_size, out_left;
    char    *out_p, *out_buf, *tmp_buf;
    size_t  bsz, result = 0;

    *out        = NULL;
    *out_len    = 0;

    if (iv == (iconv_t) -1)
    {
        return false;
    }

    in_left     = in_len;
    out_left    = in_len + 64; /* Avoid realloc() most cases */
    out_size    = 0;
    bsz         = out_left;
    out_buf     = (char*) emalloc(bsz + 1);
    out_p       = out_buf;

    while (in_left > 0)
    {
        result  = iconv(iv, (char**)&in_p, &in_left, (char**)&out_p, &out_left);
        out_size    = bsz - out_left;
        if (result = (size_t) -1)
        {
            if (errno == E2BIG && in_left > 0)
            {
                bsz += in_len;
                tmp_buf = (char*) erealloc(out_buf, bsz + 1);
                out_p   = out_buf = tmp_buf;
                out_p   += out_size;
                out_left    = bsz - out_size;
                continue;
            }
        }
        break;
    }

    if (result != (size_t) - 1)
    {
        /* flush the shift-out sequences */
        while (true)
        {
            result  = iconv(iv, NULL, NULL, (char**) &out_p, &out_left);
            out_size    = bsz - out_left;

            if (result != (size_t) -1)
            {
                break;
            }
            if (errno == E2BIG)
            {
                bsz += 16;
                tmp_buf = (char*) erealloc(out_buf, bsz);

                out_p   = out_buf   = tmp_buf;
                out_p   += out_size;
                out_left    = bsz   - out_size;
            } else {
                break;
            }
        }
    }
    *out_p      = '\0';
    *out        = out_buf;
    *out_len    = out_size;
    return true;
}

#endif
