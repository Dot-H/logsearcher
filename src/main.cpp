#include <iostream>
#include <fstream>

#include "cli.hh"
#include "commands.hh"

CmdBuilder::cmd_mapper CmdBuilder::cmds = {
    {"top", CmdBuilder::cmdBuilder<Top, int>}
};

static void printCmd(const std::vector<std::string>& vect) {
    for (const auto& arg : vect)
        std::cout << arg << '\n';
}

static bool parseArguments(int argc, char *argv[], Environment &env) { 
    if (argc > 2) {
        std::cerr << "Usage: ./logsearcher [FILE]" << std::endl;
        return false;
    }

    if (argc == 2)
        env.path = argv[1];

    return true;
}

int main(int argc, char *argv[]) {
    Environment env = { "", std::cout, -1, Cli(std::cin) };
    std::vector<std::string> args;
    if (env.cli.getcmdline(args)) {
        auto builder = CmdBuilder::cmds.at(args[0]);
        auto cmd = builder(env, args);
        (*cmd)();
    }
}
