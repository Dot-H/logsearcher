#pragma once

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
CmdBuilder::cmd_ptr CmdBuilder::cmdBuilder(Environment &,
                                           std::vector<std::string>) {
  throw std::invalid_argument("Command does not exist");
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<Top, int>(Environment &env,
                                 std::vector<std::string> args) {
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
CmdBuilder::cmdBuilder<File, const std::string &>(Environment &env,
                                 std::vector<std::string> args) {
    if (args.size() < 2)
        throw std::invalid_argument("File expect a file as argument");

    auto file = std::make_shared<File>(env, args[1]);
    return file;
}

void Top::operator() () const {
    std::cout << n << std::endl;
    std::cout << ts.first << "--" << ts.second << std::endl;
};
