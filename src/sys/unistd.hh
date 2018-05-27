#pragma once

#include "wrapper.hh"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace sys {
static inline auto open = make_wrapper((int (*)(const char *, int))::open);
static inline auto read = make_wrapper(::read);
static inline auto write = make_wrapper(::write);
static inline auto close = make_wrapper(::close);
static inline auto fstat = make_wrapper(::fstat);
} // namespace sys
