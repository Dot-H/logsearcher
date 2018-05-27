#pragma once

#include <functional>
#include <system_error>

/**
  * \brief Syscall wrapper in order to get a more c++ style syscalls
  */
namespace sys {
template <typename RetVal, typename... Args>
class SysWrapper {
    public:
        SysWrapper(RetVal syscall(Args...))
            : syscall_(syscall) {}

        RetVal operator()(Args... args) {
            RetVal return_value;
            do {
                return_value = syscall_(args...);
            } while (errno == EINTR && !(errno = 0));

            if (return_value == RetVal(-1))
                throw std::system_error(errno, std::system_category());

            return return_value;
        }

    private:
        std::function<RetVal(Args...)> syscall_;
};

template <typename RetVal, typename... Args>
SysWrapper<RetVal, Args...> make_wrapper(RetVal syscall(Args...)) {
    return SysWrapper<RetVal, Args...>(syscall);
}
} // namespace sys
