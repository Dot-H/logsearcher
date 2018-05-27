#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <utility>

#include "unistd.hh"
#include "logtime.hh"
#include "logblock.hh"

/**
  * \brief Make the interface between a file and the user.
  *
  * \note  The file is supposed well formated and with at least one log.
  *        The file is treated as a group of page-aligned block. When an
  *        offset is asked, it is the index of a block.
  *        A bound is the start of the first / last log on a range.
  */
class LogFile {
    public:
        using blk_ptr = std::shared_ptr<LogBlock>;
        using timerange = std::pair<LogTime, LogTime>;
        using blk_bounds = std::pair<blk_ptr, blk_ptr>;

        LogFile(const std::string &filename);
        LogFile(const LogFile &) = delete;

        ~LogFile() { forceClose(); };

        friend std::ostream &operator<<(std::ostream &os, const LogFile &logs);

        /**
         * \brief  Make a binary search inside the file (cut in blocks) until
         *         finding the block where @time is.
         * \param  minBlkIdx inclusive lower bound of the search.
         * \param  maxBlkIdx inclusive upper bound of the search.
         * \return Return a pointer to the found block on success and nullptr
         *         on failure.
         */
        blk_ptr searchBlock(const LogTime &time, std::size_t minBlkIdx,
                            std::size_t maxBlkIdx) const;

        /**
         * \brief  Search in the file the pair of block containing respectively
         *         the lower LogTime and the upper LogTime.
         * \param  interval Respectively the lower and upper researched
         *         LogTime.
         * \param  startBlkIdx Block index from where the binary search
         *         will start.
         * \return Return a pair of pointers on the blocks representing the
         *         bounds of the interval.
         * \note   If a bound inside @interval is not present in the file
         *         (meaning too big or too small), the corresponding entry in
         *         the returned pair is null.
         */
        blk_bounds searchLimitBlocks(const timerange &interval,
                                     std::size_t startBlkIdx) const;

        /* Getters */
        const std::string &filename() const { return filename_; }
        int fd() const { return fd_; }
        std::size_t pagesize() const { return pagesize_; }
        std::size_t size() const { return size_; }
        std::size_t nblock() const { return nblock_; }

    private:
        std::string filename_;
        int fd_;
        std::size_t pagesize_;
        std::size_t size_;
        std::size_t nblock_;

        /**
         * \brief Close @fd_ if open and ignores error if any.
         */
        void forceClose() {
          if (!fd_)
              return;

          try {
              sys::close(fd_);
          } catch(std::system_error &) {}
      }
};
