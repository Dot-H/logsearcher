#include "environment.hh"
#include "unistd.hh"

#include <iostream>

void Environment::forceClose() {
    if (!fd_)
        return;

    try {
        sys::close(fd_);
    } catch(std::system_error &) {}
}

void Environment::loadFile(const std::string &filename) {
    forceClose();
    fd_ = sys::open(filename.c_str(), O_RDONLY);
    path_ = filename;
    std::cout << "fd: " << fd_ << std::endl;
}
