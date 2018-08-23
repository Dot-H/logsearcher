#include "commands.hh"

LogTime CmdBuilder::parseLogTime(std::string str) {
    std::string::size_type n = str.find(":");
    if (n == std::string::npos) {
        if (str.size() != DATE_LEN) // Invalid date
            throw LogTimeException("Invalid time format: " + str);
        else // No hour specified
            str.append(":00:00:00");
    }

    return LogTime(str);
}

CmdBuilder::timerange CmdBuilder::parseTimerange(const std::string &str) {
    std::string::size_type n = str.find(".");
    if (n == std::string::npos)
        throw std::invalid_argument("Argument is not a timerange: " + str);

    timerange time = std::make_pair(LogTime(MIN_TIME), LogTime(MAX_TIME));
    if (str[0] != '.')
        time.first = parseLogTime(str.substr(0, n));
    if (n != str.size() - 1)
        time.second = parseLogTime(str.substr(n + 1));

    if (time.first > time.second)
        throw std::invalid_argument("Invalid timerange: left value cannot "
                                    "be inferior to the right one");
    return time;
}

template <class C, typename... Args>
CmdBuilder::cmd_ptr CmdBuilder::cmdBuilder(Environment &, const string_vec&) {
  throw std::invalid_argument("Command does not exist");
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<Top, int>(Environment &env, const string_vec &args) {
    if (env.logs() == nullptr)
        throw std::invalid_argument("No file loaded");

    timerange ts = std::make_pair(MIN_TIME, MAX_TIME);
    if (args.size() < 2)
        throw std::invalid_argument("top expect a number as argument");
    if (args.size() == 3)
        ts = parseTimerange(args[2]);

    int ntop = std::stoi(args[1]);
    auto top = std::make_shared<Top>(env, ntop, ts);
    return top;
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<Count>(Environment &env, const string_vec &args) {
    if (env.logs() == nullptr)
        throw std::invalid_argument("No file loaded");

    timerange ts = std::make_pair(MIN_TIME, MAX_TIME);
    if (args.size() == 2)
        ts = parseTimerange(args[1]);

    auto count = std::make_shared<Count>(env, ts);
    return count;
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<File, const std::string &>(Environment &env,
                                                  const string_vec &args) {
    if (args.size() < 2)
        throw std::invalid_argument("File expect a file as argument");

    auto file = std::make_shared<File>(env, args[1]);
    return file;
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<Quit>(Environment &env, const string_vec &) {
    auto quit = std::make_shared<Quit>(env);
    return quit;
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<Help>(Environment &env, const string_vec &) {
    auto help = std::make_shared<Help>(env);
    return help;
}
