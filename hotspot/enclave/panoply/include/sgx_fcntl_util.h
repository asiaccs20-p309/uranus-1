#ifndef SGX_FCNTL_UTIL_H
#define SGX_FCNTL_UTIL_H 1

#include "struct/sgx_fcntl_struct.h"
#include "sgx_stdio_util.h"

#ifdef __cplusplus
extern "C" {
#endif

int open(const char *pathname, int flags, ...);

int creat(const char *pathname, unsigned int mode);

int openat(int dirfd, const char *pathname, int flags, ...);

int fcntl(int fd, int cmd, ...);

#ifdef __cplusplus
}
#endif

#endif
