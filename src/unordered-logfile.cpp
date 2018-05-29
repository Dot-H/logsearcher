#include <algorithm>

#include "unordered-logfile.hh"

/**
  * \brief Guard used to make sure the mapping will be unmapped when
  *        leaving the scope it has been declared in.
  */
struct MmapGuard {
    MmapGuard(void *addr, std::size_t length, int prot, int flags,
            int fd, std::size_t offset)
        : addr(sys::mmap(addr, length, prot, flags, fd, offset)),
          length(length) {}
    ~MmapGuard() { sys::munmap(addr, length); }

    void *addr;
    std::size_t length;
};

UnorderedLogFile::UnorderedLogFile(const std::string &filename)
        : LogFile(filename) {
    parseFile();
}

/**
  * \brief Search a char in a char array without bound checking.
  * \param arr Char array
  * \param c Searched character
  * \return Return the index of the found element.
  * \note   This function assumes the char IS IN THE ARRAY. There is
  *         no bound checking.
  */
static std::size_t unsafeFind(const char *arr, char c) {
    std::size_t n = 0;
    while (arr[n] != c) // TODO: vectorized
        ++n;

    return n;
}

/* TODO: Add support of MAP_HUGETLB if available */
void UnorderedLogFile::parseFile() {
    std::cerr << "Loading file... " << std::endl;
    MmapGuard mapping(NULL, size_, PROT_READ, MAP_SHARED, fd_, 0);
    const char *file = static_cast<const char *>(mapping.addr);
    std::size_t idx = 0;
    while (idx < size_) {
        LogTime query_time(file + idx);
        idx += TIME_LEN + 1;

        std::size_t queryLen = unsafeFind(file + idx, '\n');
        std::string query(file + idx, queryLen);

        queries_map_[query_time][query] += 1;
        idx += queryLen + 1;
    }
}

template <typename Iterator>
void UnorderedLogFile::sumQueryCounters(Iterator start, Iterator end,
                                         query_cnter &sum) const {
    for (; start != end; ++start)
        for (const auto &cnter : start->second)
            sum[cnter.first] += cnter.second;
}

void UnorderedLogFile::populateTopVector(const query_cnter &sum,
                                         std::vector<cnter_pair> &top) const {
    std::partial_sort_copy(sum.begin(), sum.end(), top.begin(), top.end(),
                           [](const auto &lh, const auto &rh) {
                               return lh.second > rh.second;
                           });
}

std::ostream &UnorderedLogFile::top(std::ostream &os, std::size_t n,
                                    const timerange &range) const {
    const auto start = queries_map_.lower_bound(range.first);
    if (start == queries_map_.end())
      throw std::invalid_argument("No query found in this time range");

    const auto end = queries_map_.upper_bound(range.second);
    std::vector<cnter_pair> top;

    { // Relax the memory from sum as soon as we are done using it
        query_cnter sum;
        sumQueryCounters(start, end, sum);

        top.resize(n <= sum.size() ? n : sum.size());
        populateTopVector(sum, top);
    }

    auto el = top.begin();
    for (; el != top.end()-1; ++el)
        os << (*el).first << " " << (*el).second << '\n';
    if (el != top.end())
        os << (*el).first << " " << (*el).second;

    return os;
}

std::ostream &UnorderedLogFile::count(std::ostream &os,
                                      const timerange &range) const {
    const auto start = queries_map_.lower_bound(range.first);
    if (start == queries_map_.end())
      throw std::invalid_argument("No query found in this time range");

    const auto end = queries_map_.upper_bound(range.second);
    std::vector<cnter_pair> top;

    query_cnter sum;
    sumQueryCounters(start, end, sum);

    os << sum.size();
    return os;
}
