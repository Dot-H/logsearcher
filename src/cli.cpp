#include <sstream>
#include <istream>

#include "cli.hh"
#include "commands.hh"

CmdBuilder::cmd_mapper CmdBuilder::cmds = {
    {"top", std::make_pair(CmdBuilder::cmdBuilder<Top, int>,
     "Output the top N popular queries (one per line) that have been done "
     "during a specific time range")},
    {"count", std::make_pair(CmdBuilder::cmdBuilder<Count>,
     "Output the number of distinct queries that have been done during a "
     "specific time range")},
    {"file", std::make_pair(CmdBuilder::cmdBuilder<File, const std::string &>,
     "Load a new log file")},
    {"quit", std::make_pair(CmdBuilder::cmdBuilder<Quit>, "Quit the command "
     "line interface")},
    {"help", std::make_pair(CmdBuilder::cmdBuilder<Help>, "Display this message")}
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
    env_.setRunning(true);
    while (env_.isRunning() && getcmdline(args)) {
        try {
            auto builder = CmdBuilder::cmds.at(args[0]).first;
            auto cmd = builder(env_, args);
            (*cmd)();
        } catch (const std::out_of_range &e) {
            std::cerr << "Invalid command" << std::endl;
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
