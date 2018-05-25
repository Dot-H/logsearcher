#pragma once

#include "commands.hh"

template <class C, typename ...T>
CmdBuilder::cmd_ptr CmdBuilder::cmdBuilder(std::vector<std::string> args) {
    std::cout << "Toto" << std::endl;
    throw "Command does not exist";
}

template <>
CmdBuilder::cmd_ptr CmdBuilder::cmdBuilder<Top, int>(std::vector<std::string> args) {
    auto top = std::make_shared<Top>(10);
    return top;
}
