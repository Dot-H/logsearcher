#pragma once

#include <ostream>
#include <string>

class Environment {
    public:
        Environment(std::ostream &out)
            : out_(out) {};
        ~Environment() { forceClose(); }

        Environment(std::ostream &out, const char *filename)
            : out_(out) {
          loadFile(filename);
        }

        Environment(const Environment &) = delete;

        /* Setters */
        /**
          * \brief Change the currend working file (@fd_ and @path_).
          *        @filename is open with the flags O_RDONLY.
          * \param filename name of a valid file
          * \throw system_error Thrown if the open(2) call fail. Describes
          *        the reasons of the failure.
          */
        void loadFile(const std::string &filename);

    private:
        std::ostream &out_;
        std::string path_;
        int fd_ = 0;

        /**
          * \brief Close @fd_ if open and ignores error if any.
          */
        void forceClose();
};
