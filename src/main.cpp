#include <iostream>
#include <fstream>

#include "cli.hh"

void printCmd(const std::vector<std::string>& vect) {
    for (const auto& arg : vect)
        std::cout << arg << '\n';
}

int main(void) {
    Cli cli(std::cin);
    std::vector<std::string> cmd;

    if (cli.getcmd(cmd))
        printCmd(cmd);
}
