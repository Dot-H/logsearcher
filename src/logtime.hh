#pragma once

#include <string>
#include <regex>
#include <exception>

/* Length of LogTime string. Ex: "2015-08-01 00:03:46" */
#define TIME_LEN 19

/* Length of LogTime date string. Ex: "2015-08-01" */
#define DATE_LEN 10

/* Length of LogTime hour string. Ex: "00:03:46" */
#define HOUR_LEN 8

class LogTimeException : public std::exception {
    public:
        LogTimeException(std::string msg)
            : msg_(msg) {}

        virtual const char* what() const throw() {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

class LogTime {
    public:
        LogTime(const char *rawtime)
            : date_(std::string(rawtime, DATE_LEN)),
              hour_(std::string(rawtime + DATE_LEN + 1, HOUR_LEN)) {
                  checkDate(); checkHour();
              }
        LogTime(std::string &date, std::string &hour)
            :date_(date), hour_(hour) {checkDate(); checkHour();}
        LogTime(const LogTime &time)
            :date_(time.date_), hour_(time.hour_) {}

       bool operator<(const LogTime &rh) const;
       bool operator>(const LogTime &rh) const;
       bool operator<=(const LogTime &rh) const;
       bool operator>=(const LogTime &rh) const;
       bool operator==(const LogTime &rh) const;

       friend std::ostream &operator<<(std::ostream &os, const LogTime &time);

       /**
        *  \return True if min <= this <= max. Else otherwise.
        */
       bool isInside(const LogTime &min, const LogTime &max) const {
           return *this >= min && *this <= max;
       }

   private:
        std::string date_;
        std::string hour_;

        /**
        * \brief Check for the validity of @date_
        * \throw Throw a LogTimeException if @date_ is invalid
        */
        void checkDate() {
           if (!std::regex_match(date_, std::regex("[0-9]{4}(-[0-9]{2}){2}")))
               throw LogTimeException("Invalid date: " + date_);
        }

        /**
        * \brief Check for the validity of @hour_
        * \throw Throw a LogTimeException if @hour_ is invalid
        */
        void checkHour() {
           if (!std::regex_match(hour_, std::regex("[0-9]{2}(:[0-9]{2}){2}")))
               throw LogTimeException("Invalid hour: " + hour_);
        }
};
