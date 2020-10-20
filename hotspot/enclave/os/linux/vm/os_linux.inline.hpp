/*
 * Copyright (c) 1999, 2013, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef OS_LINUX_VM_OS_LINUX_INLINE_HPP
#define OS_LINUX_VM_OS_LINUX_INLINE_HPP

#include "runtime/atomic.inline.hpp"
#include "runtime/orderAccess.inline.hpp"
#include "runtime/os.hpp"

// System includes

#include <unistd.h>
#include <syscall_include/unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netdb.h>

inline void* os::thread_local_storage_at(int index) {
  return _thread_data_cache[index];
}

inline const char* os::file_separator() {
  return "/";
}

inline const char* os::line_separator() {
  return "\n";
}

inline const char* os::path_separator() {
  return ":";
}

// File names are case-sensitive on windows only
inline int os::file_name_strcmp(const char* s1, const char* s2) {
  return strcmp(s1, s2);
}

inline bool os::obsolete_option(const JavaVMOption *option) {
  return false;
}

inline bool os::uses_stack_guard_pages() {
  return true;
}

inline bool os::allocate_stack_guard_pages() {
  assert(uses_stack_guard_pages(), "sanity check");
  return true;
}


// On Linux, reservations are made on a page by page basis, nothing to do.
inline void os::pd_split_reserved_memory(char *base, size_t size,
                                      size_t split, bool realloc) {
}


// Bang the shadow pages if they need to be touched to be mapped.
inline void os::bang_stack_shadow_pages() {
}

inline void os::dll_unload(void *lib) {
  D_WARN_Unimplement;
}

inline const int os::default_file_open_flags() { return 0;}

inline DIR* os::opendir(const char* dirname)
{
  assert(dirname != NULL, "just checking");
  return ::opendir(dirname);
}

inline int os::readdir_buf_size(const char *path)
{
  return NAME_MAX + sizeof(dirent) + 1;
}

inline jlong os::lseek(int fd, jlong offset, int whence) {
  D_WARN_Unimplement;
}

inline int os::fsync(int fd) {
  D_WARN_Unimplement;
}

inline char* os::native_path(char *path) {
  return path;
}

inline int os::ftruncate(int fd, jlong length) {
  D_WARN_Unimplement;
}

inline struct dirent* os::readdir(DIR* dirp, dirent *dbuf)
{
  dirent* p;
  int status;
  assert(dirp != NULL, "just checking");

  // NOTE: Linux readdir_r (on RH 6.2 and 7.2 at least) is NOT like the POSIX
  // version. Here is the doc for this function:
  // http://www.gnu.org/manual/glibc-2.2.3/html_node/libc_262.html

  if((status = ::readdir_r(dirp, dbuf, &p)) != 0) {
    errno = status;
    return NULL;
  } else
    return p;
}

inline int os::closedir(DIR *dirp) {
  assert(dirp != NULL, "argument is NULL");
  return ::closedir(dirp);
}

// macros for restartable system calls

#define RESTARTABLE(_cmd, _result) do { \
    _result = _cmd; \
  } while(((int)_result == OS_ERR) && (errno == EINTR))

#define RESTARTABLE_RETURN_INT(_cmd) do { \
  int _result; \
  RESTARTABLE(_cmd, _result); \
  return _result; \
} while(false)

inline bool os::numa_has_static_binding()   { return true; }
inline bool os::numa_has_group_homing()     { return false;  }

inline size_t os::restartable_read(int fd, void *buf, unsigned int nBytes) {
  size_t res;
  RESTARTABLE( (size_t) ::read(fd, buf, (size_t) nBytes), res);
  return res;
}

inline size_t os::write(int fd, const void *buf, unsigned int nBytes) {
  size_t res;
  RESTARTABLE((size_t) ::write(fd, buf, (size_t) nBytes), res);
  return res;
}

inline int os::close(int fd) {
  return ::close(fd);
}

inline int os::socket_close(int fd) {
  return ::close(fd);
}

inline int os::socket(int domain, int type, int protocol) {
  D_WARN_Unimplement;
}

inline int os::recv(int fd, char* buf, size_t nBytes, uint flags) {
  D_WARN_Unimplement;
}

inline int os::send(int fd, char* buf, size_t nBytes, uint flags) {
  D_WARN_Unimplement;
}

inline int os::raw_send(int fd, char* buf, size_t nBytes, uint flags) {
  D_WARN_Unimplement;
}

inline int os::timeout(int fd, long timeout) {
  D_WARN_Unimplement;
}

inline int os::listen(int fd, int count) {
  D_WARN_Unimplement;
}

inline int os::connect(int fd, struct sockaddr* him, socklen_t len) {
  D_WARN_Unimplement;
}

inline int os::accept(int fd, struct sockaddr* him, socklen_t* len) {
  // Linux doc says this can't return EINTR, unlike accept() on Solaris.
  // But see attachListener_linux.cpp, LinuxAttachListener::dequeue().
  D_WARN_Unimplement;
}

inline int os::recvfrom(int fd, char* buf, size_t nBytes, uint flags,
                        sockaddr* from, socklen_t* fromlen) {
  D_WARN_Unimplement;
}

inline int os::sendto(int fd, char* buf, size_t len, uint flags,
                      struct sockaddr* to, socklen_t tolen) {
  D_WARN_Unimplement;
}

inline int os::socket_shutdown(int fd, int howto) {
  D_WARN_Unimplement;
}

inline int os::bind(int fd, struct sockaddr* him, socklen_t len) {
  D_WARN_Unimplement;
}

inline int os::get_sock_name(int fd, struct sockaddr* him, socklen_t* len) {
  D_WARN_Unimplement;
}

inline int os::get_host_name(char* name, int namelen) {
  D_WARN_Unimplement;
}

inline struct hostent* os::get_host_by_name(char* name) {
  D_WARN_Unimplement;
}

inline int os::get_sock_opt(int fd, int level, int optname,
                            char* optval, socklen_t* optlen) {
  D_WARN_Unimplement;
}

inline int os::set_sock_opt(int fd, int level, int optname,
                            const char* optval, socklen_t optlen) {
  D_WARN_Unimplement;
}

#endif // OS_LINUX_VM_OS_LINUX_INLINE_HPP
