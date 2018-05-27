#include "logblock.hh"
#include "logtime.hh"

std::ostream &operator<<(std::ostream &os, const LogBlock &logs) {
    os << '[' << LogTime(static_cast<const char *>(logs.firstLog()));
    os << " - " << LogTime(static_cast<const char *>(logs.lastLog())); 
    os << ']' << '\n';
    os << "Block idx: "  << logs.idx_ << '\n';
    os << std::string(logs.map_ + logs.lowerBound_,
                      logs.upperBound_ - logs.lowerBound_);
    return os;
}

std::size_t LogBlock::lowerBoundOff() {
    std::size_t start = 0;
    while (map_[start++] != '\n' && start < size_); // Put start on a valid date

    return start;
}

std::size_t LogBlock::upperBoundOff() {
    std::size_t end = size_ - 1;
    while (map_[end--] != '\n' && end > 0); // Trash the incomplete line
    while (map_[end - 1] != '\n' && end > 0) // Put the index on a valid date
        --end;

    return end;
}
