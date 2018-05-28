#include "logfile.hh"

LogFile::LogFile(const std::string &filename)
        : filename_(filename) {
    fd_ = sys::open(filename.c_str(), O_RDONLY);

    struct stat sb;
    sys::fstat(fd_, &sb);
    size_ = sb.st_size;
}

std::ostream &operator<<(std::ostream &os, const LogFile &logs) {
    os << "Filename: "  << logs.filename_ << " (fd: " << logs.fd_ << ")\n";
    os << "Size: "      << logs.size_ << '\n';
    return os;
}
