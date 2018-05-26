#pragma once

struct Environment {
    std::string path;
    std::ostream &out;
    int fd;
    Cli cli;
};
