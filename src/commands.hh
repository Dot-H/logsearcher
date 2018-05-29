#pragma once

#include <unordered_map>
#include <memory>
#include <functional>

#include "logtime.hh"
#include "unordered-logfile.hh"

class Environment;

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
    using string_vec = std::vector<std::string>;
    using builder_func = std::function<cmd_ptr(Environment&, string_vec)>;
    using cmd_mapper = std::unordered_map<std::string, builder_func>;
    static cmd_mapper cmds;

    template <class C, typename... Args>
    static cmd_ptr cmdBuilder(Environment &env, const string_vec &args);

    using time_opt = std::optional<LogTime>;
    using timerange = std::pair<LogTime, LogTime>;
    static LogTime parseLogTime(std::string str);
    static timerange parseTimerange(const std::string &str);
};

/**
  * \brief Functor of the 'top' command in the cli.
  *        This command is used to print the top n log from the file loaded
  *        in the environment.
  */
struct Top : public Cmd {
    Top(const Environment &env, int n, CmdBuilder::timerange ts)
        : Cmd("top"), env(env), n(n), ts(ts) {}

    void operator() () const override {
        env.logs()->top(env.out(), n, ts) << std::endl;
    }

    const Environment &env;
    const int n;
    CmdBuilder::timerange ts;
};

/**
  * \brief Functor of the 'count' command in the cli.
  *        This command is used to output the number of distinct queries
  *        that have been done during a specific time range..
  */
struct Count : public Cmd {
    Count(Environment &env, CmdBuilder::timerange ts)
        : Cmd("cnt"), env(env), ts(ts) {}

    void operator() () const override {
        env.logs()->count(env.out(), ts) << std::endl;
    }

    const Environment &env;
    CmdBuilder::timerange ts;
};

/**
  * \brief Functor of the 'file' command in the cli.
  *        This command is used to change the current working file in the
  *        environment.
  */
struct File : public Cmd {
    File(Environment &env, const std::string &filename)
        : Cmd("file"), env(env), filename(filename) {}

    void operator() () const override {
        env.loadFile<UnorderedLogFile>(filename);
    }

    Environment &env;
    const std::string filename;
};

#include "commands.hxx"
