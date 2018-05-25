#include <iostream>
#include <fstream>

#include "cli.hh"
#include "commands.hh"

CmdHelper::cmd_mapper CmdHelper::cmds = {
    {"top", &makeCmd<Top, int>}
};

void printCmd(const std::vector<std::string>& vect) {
    for (const auto& arg : vect)
        std::cout << arg << '\n';
}


int main(void) {
    Cli cli(std::cin);
    std::vector<std::string> args;

    if (cli.getcmdline(args)) {
        auto maker = CmdBuilder::cmds.at(args[0]);
        auto cmd = maker(args);
        (*cmd)();
    }
}
