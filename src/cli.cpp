#include <sstream>
#include <istream>

#include "cli.hh"
#include "commands.hh"

CmdBuilder::cmd_mapper CmdBuilder::cmds = {
    {"top", CmdBuilder::cmdBuilder<Top, int>},
    {"file", CmdBuilder::cmdBuilder<File, const std::string &>},
    {"count", CmdBuilder::cmdBuilder<Count>}
};

bool Cli::getcmdline(std::vector<std::string> &cmd) const {
    cmd.clear();
    std::string line;
    if (&is_ == &std::cin)
        printPrompt();

    std::getline(is_, line, '\n');
    if (is_.fail())
        return false;

    std::istringstream cmdstream(line);
    std::string arg;
    do {
        std::getline(cmdstream, arg, ' ');
        cmd.push_back(arg);
    } while (cmdstream.good() && !cmdstream.eof());

    return !cmdstream.fail();
}

void Cli::run() const {
    std::vector<std::string> args;
    while (getcmdline(args)) {
        try {
            auto builder = CmdBuilder::cmds.at(args[0]);
            auto cmd = builder(env_, args);
            (*cmd)();
        } catch (const std::out_of_range &e) {
            std::cerr << "Invalid command" << std::endl;
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
