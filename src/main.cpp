#include <iostream>
#include <fstream>

#include "cli.hh"

static bool parseArguments(int argc, char *argv[], Environment &env) { 
    if (argc > 2) {
        std::cerr << "Usage: ./logsearcher [FILE]" << std::endl;
        return false;
    }

    if (argc == 2)
        env.loadFile(argv[1]);

    return true;
}

int main(int argc, char *argv[]) {
    Environment env(std::cout);   
    Cli cli = Cli(env, std::cin);
    if (!parseArguments(argc, argv, env))
        return 1;

    cli.run();
}
