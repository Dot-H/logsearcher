#include <iostream>
#include <ostream>
#include <string>

#include "logtime.hh"

LogTime make_logtime(const std::string &time) {
    std::string::size_type n = time.find(' ');
    std::string date = time.substr(0, n);
    std::string hour = time.substr(n + 1);

    return LogTime(date, hour);
}

template <class func>
void compare(std::string op, const LogTime &lh, const LogTime &rh,
             func f = func()) {
  std::cout << lh << ' ' << op << ' ' << rh << " = " << f(lh, rh);
  std::cout << '\n';
}

int main(int argc, char *argv[]) {
    if (argc < 3)
        return 1;

   std::cout << std::boolalpha;
    for (int i = 1; i < argc - 1; ++i) {
       LogTime lh = make_logtime(std::string(argv[i]));
       std::cout << "[" << lh << "]" << '\n';
       for (int j = 1; j < argc; ++j) {
           LogTime rh = make_logtime(std::string(argv[j]));
           compare<std::less<LogTime>>(std::string("<"), lh, rh);
           compare<std::greater<LogTime>>(std::string(">"), lh, rh);
           compare<std::greater_equal<LogTime>>(std::string(">="), lh, rh);
           compare<std::less_equal<LogTime>>(std::string("<="), lh, rh);
           compare<std::equal_to<LogTime>>(std::string("=="), lh, rh);
       }
    }
};
