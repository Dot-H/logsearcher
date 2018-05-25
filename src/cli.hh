#pragma once

#include <iostream>
#include <istream>
#include <string>
#include <vector>

/**
 * \brief Command line interface class. Used to parse an input stream line
 *        by line.
 * \note  There is no intelligence in the parsing. Quotes are not supported.
 */
class Cli {
    public:
        Cli(std::istream &is)
            : is_(is), prompt_("> ") {}

        Cli(std::istream &is, const std::string &prompt)
            : is_(is), prompt_(prompt) {}

        /**
         * \brief  Parse a line from the attribute @is_ and fill the @cmd
         *         with the parsed line.
         * \param  cmd The vector to fill with the arguments 
         * \return True on success and false if an io error occured
         * \note   The line is parsed with a space as delimiter
         */
        bool getcmdline(std::vector<std::string> &cmd);

        /**
         * Output the attribute @prompt_ to stderr
         */
        void printPrompt() { std::cerr << prompt_ << std::flush; };

        /* Getters */
        const std::istream &is() const { return is_; };
        const std::string &prompt() const { return prompt_; };
    private:
        std::istream &is_; // Can't live without an input stream
        std::string prompt_;
};
