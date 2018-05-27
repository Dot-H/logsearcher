#include "logfile.hh"

LogFile::LogFile(const std::string &filename)
        : filename_(filename) {
    fd_       = sys::open(filename.c_str(), O_RDONLY);
    pagesize_ = sys::sysconf(_SC_PAGE_SIZE);

    struct stat sb;
    sys::fstat(fd_, &sb);
    size_   = sb.st_size;
    nblock_ = size_ / pagesize_ + (size_ % pagesize_ != 0);
}

std::ostream &operator<<(std::ostream &os, const LogFile &logs) {
    os << "Filename: "  << logs.filename_ << " (fd: " << logs.fd_ << ")\n";
    os << "Size: "      << logs.size_ << '\n';
    os << "Page size: " << logs.pagesize_ << '\n';
    os << "Number of page-aligned block: " << logs.nblock_;
    return os;
}

/* TODO: check if make_shared is not too expensive. If it is, add counting
         Block's map_ and add the copy constructor in order to make_shared
         only when the block is found.
*/
LogFile::blk_ptr LogFile::searchBlock(const LogTime &time,
                                      std::size_t minBlkIdx,
                                      std::size_t maxBlkIdx) const {
    while (minBlkIdx <= maxBlkIdx) {
        std::size_t mid = (minBlkIdx + maxBlkIdx) / 2;
        blk_ptr blk = std::make_shared<LogBlock>(mid, pagesize_, MAP_SHARED,
                                                 fd_, mid * pagesize_);

        /* FIXME: This test should be avoid with the heuristic optimization.
                  The idea is to lookup the ranges between the logs time of
                  the first and last block of the file in order to make smarter
                  choices with the initials min/maxBlkIdx. Therefore, since we
                  know the bounds of the first block of the file before
                  making the search, this edge case would never happen.
        */
        LogTime lowBound = (!mid) ? LogTime(static_cast<const char *>(blk->addr()))
                                  : LogTime(static_cast<const char *>(blk->firstLog()));
        LogTime upBound(static_cast<const char *>(blk->lastLog()));
        if (time.isInside(lowBound, upBound))
            return blk;

        if (time < lowBound)
            maxBlkIdx = mid - 1;
        else
            minBlkIdx = mid + 1;
    }

    return nullptr; /* Not found */
}

static void startSearch(const LogTime &searched, const LogFile::blk_ptr &cur,
                        LogFile::blk_ptr &res, const LogFile &file) {
    LogTime lowBound(static_cast<const char *>(cur->firstLog()));
    LogTime upBound(static_cast<const char *>(cur->lastLog()));
    if (searched.isInside(lowBound, upBound))
        res = cur;
    else {
        if (searched < lowBound)
            res = file.searchBlock(searched, 0, cur->index() - 1);
        else
            res = file.searchBlock(searched, cur->index(), file.nblock() - 1);
    }
}

LogFile::blk_bounds LogFile::searchLimitBlocks(const timerange &interval,
                                               std::size_t startBlkIdx) const {
    blk_ptr blk = std::make_shared<LogBlock>(startBlkIdx, pagesize_, MAP_SHARED,
                                             fd_, startBlkIdx * pagesize_);
    blk_bounds ret(nullptr, nullptr);
    startSearch(interval.first, blk, ret.first, *this);
    if (ret.first != nullptr)
        startSearch(interval.second, ret.first, ret.second, *this);

    return ret;
}
