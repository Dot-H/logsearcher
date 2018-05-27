#pragma once

#include <iostream>
#include <istream>
#include <string>
#include <vector>

#include "environment.hh"

/**
 * \brief Command line interface class. Used to parse an input stream line
 *        by line.
 * \note  Both prompt and errors are written on stderr. The output of the
 *        commands are written in the environment.
 *        There is no intelligence in the parsing. Quotes are not supported.
 */
class Cli {
    public:
        Cli(Environment &env)
            : env_(env), is_(std::cin), prompt_("> ") {}
        Cli(Environment &env, std::istream &is)
            : env_(env), is_(is), prompt_("> ") {}
        Cli(Environment &env, std::istream &is, const std::string &prompt)
            : env_(env), is_(is), prompt_(prompt) {}

        Cli(const Cli &) = delete;

        /**
         * \brief  Ask a command line to the user while not receiving an
         *         EOF or a signal. The input will be parsed and execute
         *         with the given arguments.
         */
        void run() const;

        /**
         * \brief  Parse a line from the attribute @is_ and fill the @cmd
         *         with the parsed line.
         * \param  cmd The vector to fill with the arguments
         * \return True on success and false if an io error occured
         * \note   The line is parsed with a space as delimiter
         */
        bool getcmdline(std::vector<std::string> &cmd) const;

        /**
         * Output the attribute @prompt_ to stderr
         */
        void printPrompt() const { std::cerr << prompt_ << std::flush; };

        /* Getters */
        const std::istream &is() const { return is_; };
        const std::string &prompt() const { return prompt_; };
        Environment &env() const { return env_; };
    private:
        Environment &env_;
        std::istream &is_;
        std::string prompt_;
};
