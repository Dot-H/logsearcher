#include <iostream>
#include "logfile.hh"

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        try {
            std::cout << "[" << argv[i] << "]" << '\n';
            std::cout << LogFile(argv[i]) << '\n';
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
}
