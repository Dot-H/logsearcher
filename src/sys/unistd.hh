#pragma once

#include "wrapper.hh"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

namespace sys {
static inline auto open = make_wrapper((int (*)(const char *, int))::open);
static inline auto read = make_wrapper(::read);
static inline auto write = make_wrapper(::write);
static inline auto close = make_wrapper(::close);
static inline auto fstat = make_wrapper(::fstat);
static inline auto mmap = make_wrapper(::mmap);
static inline auto munmap = make_wrapper(::munmap);
static inline auto sysconf = make_wrapper(::sysconf);
} // namespace sys
