#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

/**
  * \brief Exposes a common functor for all the registered commands
  */
class Cmd {
    public:
        Cmd(const std::string &name)
            : name_(name) {}

        virtual void operator() () const = 0;

        const std::string &name() const { return name_; };
    private:
        const std::string name_;
};

/**
  * \brief Contains a map associating a command's name with its builder
  *        A builder takes a vector filled with the command's arguments
  *        as parameter. It will parse the arguments and return the functor
  *        corresponding to the command as a Cmd type.
  */
struct CmdBuilder {
    using cmd_ptr = std::shared_ptr<Cmd>;
    using builder_func = std::function<cmd_ptr(std::vector<std::string>)>;
    using cmd_mapper = std::unordered_map<std::string, builder_func>;
    static cmd_mapper cmds;

    template <class C, typename ...T>
    static cmd_ptr cmdBuilder(std::vector<std::string> args);
};


struct Top : public Cmd {
    Top(int n)
        : Cmd("top"), n(n) {}

    void operator() () const override {
        std::cout << n << std::endl;
    };

    const int n;
};

#include "commands.hxx"
