#pragma once

#include <memory>
#include <ostream>

#include "logfile.hh"

class Environment {
    using logfile_ptr = std::shared_ptr<LogFile>;
    public:
        Environment(std::ostream &out)
            : out_(out), logs_(nullptr) {}

        Environment(const Environment &) = delete;

        /**
         * \brief Change the currend working file (@fd_ and @path_).
         *        @filename is open with the flags O_RDONLY.
         * \param filename name of a valid file
         * \throw system_error Thrown if the open(2) call fail. Describes
         *        the reasons of the failure.
         */
        template<class F> /* TODO: enable_if extends LogFile */
        void loadFile(const std::string &filename) {
            logs_ = nullptr; // Force the delete in order to free the fd
            logs_ = std::make_shared<F>(filename);
        }

        /* Getters */
        const std::string &filename() { return logs_->filename(); }
        const logfile_ptr &logs() const { return logs_; }
        std::ostream &out() const { return out_; }
        bool isRunning() const { return running_; }

        /* Setters */
        void setRunning(bool value) { running_ = value; }

    private:
        std::ostream &out_;
        logfile_ptr logs_;
        bool running_ = false;
};
