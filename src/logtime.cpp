#include "logtime.hh"

std::ostream &operator<<(std::ostream &os, const LogTime &time) {
    os << time.date_ << ' ' << time.hour_;
    return os;
}

bool LogTime::operator<(const LogTime &rh) const {
    if (date_ > rh.date_)
        return false;

    return date_ < rh.date_ || hour_ < rh.hour_;
}

bool LogTime::operator>(const LogTime &rh) const {
    if (date_ < rh.date_)
        return false;

    return date_ > rh.date_ || hour_ > rh.hour_;
}
bool LogTime::operator<=(const LogTime &rh) const {
    if (date_ > rh.date_)
        return false;

    return date_ < rh.date_ || hour_ <= rh.hour_;
}

bool LogTime::operator>=(const LogTime &rh) const {
    if (date_ < rh.date_)
        return false;

    return date_ > rh.date_ || hour_ >= rh.hour_;
}

bool LogTime::operator==(const LogTime &rh) const {
    return date_ == rh.date_ && hour_ == rh.hour_;
}
