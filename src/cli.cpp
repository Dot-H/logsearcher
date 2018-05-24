#include "cli.hh"

#include <sstream>
#include <istream>

bool Cli::getcmd(std::vector<std::string> &cmd) {
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
