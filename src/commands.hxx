#pragma once

template <class C, typename... T>
CmdBuilder::cmd_ptr CmdBuilder::cmdBuilder(const Environment &env,
                                           std::vector<std::string> args) {
  throw "Command does not exist";
}

template <>
CmdBuilder::cmd_ptr
CmdBuilder::cmdBuilder<Top, int>(const Environment &env,
                                 std::vector<std::string> args) {
    if (args.size() != 2)
        throw std::invalid_argument("top expect a number as argument");

    int ntop = std::stoi(args[1]);
    auto top = std::make_shared<Top>(env, ntop);
    return top;
}
