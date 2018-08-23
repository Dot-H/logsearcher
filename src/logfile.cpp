#include "logfile.hh"

LogFile::LogFile(const std::string &filename)
        : filename_(filename) {
    fd_ = sys::open(filename.c_str(), O_RDONLY);

    struct stat sb;
    sys::fstat(fd_, &sb);
    size_ = sb.st_size;
}

std::size_t LogFile::nextLog(std::size_t idx, const char *file) const { 
    while (file[idx++] != '\n' && idx < size_); // Put idx on a valid date

    return idx;
}

std::size_t LogFile::prevLog(std::size_t idx, const char *file) const {
    while (file[idx--] != '\n' && idx > 0); // Trash the incomplete line
    while (file[idx - 1] != '\n' && idx > 0) // Put the index on a valid date
        --idx;

    return idx;
}

std::ostream &operator<<(std::ostream &os, const LogFile &logs) {
    os << "Filename: "  << logs.filename_ << " (fd: " << logs.fd_ << ")\n";
    os << "Size: "      << logs.size_ << '\n';
    return os;
}
