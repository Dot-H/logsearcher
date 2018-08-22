#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <utility>

#include "unistd.hh"
#include "logtime.hh"

/**
  * \brief Make the interface between a file and the user.
  *
  * \note  The file is supposed well formated and with at least one log.
  */
class LogFile {
    public:
        using timerange = std::pair<LogTime, LogTime>;
        LogFile(const std::string &filename);
        LogFile(const LogFile &) = delete;

        virtual ~LogFile() { forceClose(); };

        friend std::ostream &operator<<(std::ostream &os, const LogFile &logs);

        virtual std::ostream &top(std::ostream &os, std::size_t n,
                                  const timerange &range) const = 0;
        virtual std::ostream &count(std::ostream &os,
                                    const timerange &range) const = 0;

        /* FIXME: Use nextLog and prevLog in logblock */
        /**
         * \brief  Search the first linebreak from idx in order to find the 
         *         next log.
         * \return Return the idx of the starting log.
         */
        virtual std::size_t nextLog(std::size_t idx, const char *file) const;

        /**
         * \brief  Search the start of the first complete log from idx
         * \return Return the idx of the starting log.
         */
        virtual std::size_t prevLog(std::size_t idx, const char *file) const;

        /* Getters */
        const std::string &filename() const { return filename_; }
        int fd() const { return fd_; }
        std::size_t size() const { return size_; }

    protected:
        std::string filename_;
        int fd_;
        std::size_t size_;

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
