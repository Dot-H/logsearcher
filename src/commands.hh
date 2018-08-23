#pragma once

#include <functional>
#include <iomanip>
#include <memory>
#include <unordered_map>
#include <utility>

#include "environment.hh"
#include "logtime.hh"
#include "unordered-logfile.hh"

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
    using cmd_mapper =
        std::unordered_map<std::string, std::pair<builder_func, std::string>>;
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

/**
  * \brief Functor of the 'quit' command in the cli.
  *        This command is used to stop an environment.
  */
struct Quit : public Cmd {
    Quit(Environment &env)
        : Cmd("quit"), env(env) {}

    void operator()() const override { env.setRunning(false); }

    Environment &env;
};

/**
  * \brief Functor of the 'help' command in the cli.
  */
struct Help : public Cmd {
    Help(Environment &env)
        : Cmd("help"), env(env) {}

    void operator()() const override { 
       for (const auto &el : CmdBuilder::cmds) {
           std::string out = el.first + ": " + el.second.second;
           std::string pad(el.first.size() + 2, ' ');
           int off = 80 - el.first.size() - 1;
           while (int(out.size()) - off > 0) {
               env.out() << out.substr(0, off) << '\n' << pad;
               out = out.substr(off);
           }

           env.out() << out << '\n';
       }
    }

    Environment &env;
};
