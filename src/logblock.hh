#pragma once

#include <ostream>

#include "unistd.hh"

class LogBlock {
    public:
        LogBlock(std::size_t idx, std::size_t size, int flags, int fd,
                 std::size_t off)
                : idx_(idx), size_(size) {
            map_ = static_cast<char *>(sys::mmap(NULL, size,
                                                 PROT_READ, flags, fd, off));
            lowerBound_ = lowerBoundOff();
            upperBound_ = upperBoundOff();
        }
        LogBlock(const LogBlock &) = delete;
        ~LogBlock() { sys::munmap(map_, size_); }

        friend std::ostream &operator<<(std::ostream &os, const LogBlock &logs);

        /* Getters */
        std::size_t index() const { return idx_; }
        std::size_t size() const { return size_; }
        const void *addr() const { return map_; }
        const void *firstLog() const { return map_ + lowerBound_; }
        const void *lastLog() const { return map_ + upperBound_; }

    private:
        std::size_t idx_;
        std::size_t size_;
        char *map_;
        std::size_t lowerBound_;
        std::size_t upperBound_;

        /**
         * \brief  Search the lower bound inside the block
         * \note   DO NOT use this function on the first block of a file. It
         *         would miss the first log
         * \return Return the offset inside the block of the bound
         */
        std::size_t lowerBoundOff();

        /**
         * \brief  Search the upper bound inside the block
         * \return Return the offset inside the block of the bound
         */
        std::size_t upperBoundOff();
};
