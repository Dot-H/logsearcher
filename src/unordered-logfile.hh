#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "logfile.hh"

class UnorderedLogFile : public LogFile {
    public:
        using query_cnter = std::unordered_map<std::string, std::size_t>;
        UnorderedLogFile(const std::string &filename);

        std::ostream &top(std::ostream &os, std::size_t n,
                          const timerange &range) const override;

   private:
        /* Map between a timing and all the count of each query in it. */
        std::map<LogTime, query_cnter> queries_map_;

        /**
         * \brief Parse in a linear way the file in order to populate
         *        @queries_ and @queries_map_.
         */
        void parseFile();
};
