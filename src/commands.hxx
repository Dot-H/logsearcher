#pragma once

template <class C, typename... Args>
CmdBuilder::cmd_ptr CmdBuilder::cmdBuilder(Environment &,
                                           std::vector<std::string>) {
  throw std::invalid_argument("Command does not exist");
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<Top, int>(Environment &env,
                                 std::vector<std::string> args) {
    if (args.size() != 2)
        throw std::invalid_argument("top expect a number as argument");

    int ntop = std::stoi(args[1]);
    auto top = std::make_shared<Top>(env, ntop);
    return top;
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<File, const std::string &>(Environment &env,
                                 std::vector<std::string> args) {
    if (args.size() != 2)
        throw std::invalid_argument("File expect a file as argument");

    auto top = std::make_shared<File>(env, args[1]);
    return top;
}

void Top::operator() () const {
    std::cout << n << std::endl;
};
