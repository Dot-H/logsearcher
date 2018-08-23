#pragma once

#include "wrapper.hh"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

namespace sys {
inline auto open = make_wrapper((int (*)(const char *, int))::open);
inline auto read = make_wrapper(::read);
inline auto write = make_wrapper(::write);
inline auto close = make_wrapper(::close);
inline auto fstat = make_wrapper(::fstat);
inline auto mmap = make_wrapper(::mmap);
inline auto munmap = make_wrapper(::munmap);
inline auto sysconf = make_wrapper(::sysconf);
} // namespace sys
