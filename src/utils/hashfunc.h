
#ifndef __SRC_UTILS_HASHFUNC_H__
#define __SRC_UTILS_HASHFUNC_H__

#include <stdint.h>
#include <unistd.h>

namespace Utils {

class HashFunction {
  public:
    typedef uint32_t (*func)(const char *, size_t);

  public:
    static uint32_t ap(const char * key, size_t nkey);
    static uint32_t djb(const char * key, size_t nkey);
    static uint32_t elf(const char * key, size_t nkey);
    static uint32_t sax(const char * key, size_t nkey);
    static uint32_t sdbm(const char * key, size_t nkey);
    static uint32_t bkdr(const char * key, size_t nkey);
    static uint32_t murmur32(const char * key, size_t nkey, uint32_t seed=0x19851209);
    static uint64_t murmur64(const char * key, size_t nkey, uint64_t seed=0x19851209);
};

}

#endif
