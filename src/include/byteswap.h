#ifndef BYTESWAP_H_INCLUDED
#define BYTESWAP_H_INCLUDED

#if defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define bswap_16(X) OSSwapInt16(X)
#define bswap_32(X) OSSwapInt32(X)
#define bswap_64(X) OSSwapInt64(X)
#else
#include <byteswap.h>
#endif

#endif