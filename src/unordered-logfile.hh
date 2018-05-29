#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "logfile.hh"

class UnorderedLogFile : public LogFile {
    public:
        using query = std::string;
        using query_cnter = std::unordered_map<query, std::size_t>;
        using cnter_pair = std::pair<query, std::size_t>;

        UnorderedLogFile(const std::string &filename);

        std::ostream &top(std::ostream &os, std::size_t n,
                          const timerange &range) const override;
        std::ostream &count(std::ostream &os,
                            const timerange &range) const override;

    private:
        /* Map between a timing and all the count of each query in it. */
        std::map<LogTime, query_cnter> queries_map_;

        /**
         * \brief Parse in a linear way the file in order to populate
         *        @queries_ and @queries_map_.
         */
        void parseFile();

        /**
         * \brief Sum query_cnters and store it in @ret.
         * \param start Start iterator on a query_cntr
         * \param end End iterator (excluded)
         * \param ret query_cnter in which the result will be stored
         */
        template <typename Iterator>
        void sumQueryCounters(Iterator start, Iterator end,
                              query_cnter &sum) const;

        /**
         * \brief Take the top.size() greatest elements from sum and insert
         *        them in a sorted way into @top.
         */
        void populateTopVector(const query_cnter &sum,
                               std::vector<cnter_pair> &top) const;
};
