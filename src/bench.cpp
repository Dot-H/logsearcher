#include <cassert>
#include <string>
#include <memory>
#include <utility>

#include "cli.hh"
#include "commands.hh"
#include "logfile.hh"
#include "logtime.hh"
#include "unistd.hh"

#include <benchmark/benchmark.h>

#define TSV_PATH "../tests/hn_logs.tsv"
#define array_size(a) sizeof(a)

/* FIXME: double occurence of the guard */
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

struct NullBuffer : public std::streambuf {
    public:
        int overflow(int c) { return c; }
};

static NullBuffer null_buffer;
static std::ostream null_stream(&null_buffer);
static Environment env(null_stream);

class LogFileParts {
    public:
        using part_ptr = CmdBuilder::timerange *;
        LogFileParts() = default;
        LogFileParts(LogTime &lowBound) { parts_.push_back(lowBound); }

        part_ptr operator[](std::size_t n) const {
            assert(n < this->size());
            return new CmdBuilder::timerange(parts_[0], parts_[n + 1]);
        }

        void add(LogTime lt) { parts_.push_back(lt); }
        std::size_t size() const {
            assert(parts_.size());
            return parts_.size() - 1;
        }

    private:
        std::vector<LogTime> parts_;
};

static void cutFile(LogFileParts &ret, LogFile &f,
        const std::vector<double> &coefs) {
    MmapGuard mapping(NULL, f.size(), PROT_READ, MAP_SHARED, f.fd(), 0);
    const char *file = static_cast<const char *>(mapping.addr);
    ret.add(LogTime(file));

    for (std::size_t i = 0; i < coefs.size(); ++i) {
        int schLog = f.size() * coefs[i];
        ret.add(LogTime(file + f.prevLog(schLog, file)));
    }
}

static void topInput(benchmark::internal::Benchmark* b) {
    // Coef used to cut to determines parts of the tsv to bench
    static std::vector<double> cut_coefs = { 0.1, 0.5, 0.8, 1 };
    static std::vector<int64_t> top_n = { 1, 10, 100, 1000, 10000 };
    static UnorderedLogFile f(TSV_PATH);
 
    LogFileParts lfp;
    cutFile(lfp, f, cut_coefs);
    for (std::size_t i = 0; i < lfp.size(); ++i)
        for (std::size_t j = 0; j < top_n.size(); ++j)
            b->Args({reinterpret_cast<int64_t>(lfp[i]), top_n[j]});
}

static void countInput(benchmark::internal::Benchmark* b) {
    // Coef used to cut to determines parts of the tsv to bench
    static std::vector<double> cut_coefs = { 0.1, 0.5, 0.8, 1 };
    static UnorderedLogFile f(TSV_PATH);
 
    LogFileParts lfp;
    cutFile(lfp, f, cut_coefs);
    for (std::size_t i = 0; i < lfp.size(); ++i)
        b->Arg(reinterpret_cast<int64_t>(lfp[i]));
}


static Environment &create_env(const char tsv_path[]) {
    // Use to avoid the copy made by google benchmark when repeting tests
    static bool isCreated = false;
    if (!isCreated) {
        env.loadFile<UnorderedLogFile>(tsv_path);
        isCreated = !isCreated;
    }

    return env;
}

template <class orderType>
void BM_LoadFile(benchmark::State &st, const char tsv_path[], orderType) {
    for (auto _ : st) {
        Environment env(std::cout);
        env.loadFile<orderType>(tsv_path);
    }
}

void BM_Count(benchmark::State &st, Environment &env) {
    for (auto _ : st) {
        auto ts_ptr = reinterpret_cast<LogFileParts::part_ptr>(st.range(0));
        st.SetLabel("." + ts_ptr->second.to_string());
        Count cmd(env, *ts_ptr);
        cmd();
    }
}

void BM_Top(benchmark::State &st, Environment &env) {
    for (auto _ : st) {
        auto ts_ptr = reinterpret_cast<LogFileParts::part_ptr>(st.range(0));
        st.SetLabel("." + ts_ptr->second.to_string());
        Top top(env, st.range(1), *ts_ptr);
        top();
    }
}

BENCHMARK_CAPTURE(
        BM_Top, top,
        // There are no copy here because of c++17 copy ellision
        create_env(TSV_PATH))
    ->Repetitions(5)
    ->ReportAggregatesOnly(true)
    ->Unit(benchmark::kMillisecond)
    ->Apply(topInput);

BENCHMARK_CAPTURE(
    BM_Count, count,
    // There are no copy here because of c++17 copy ellision
    create_env(TSV_PATH))
    ->Repetitions(10)
    ->ReportAggregatesOnly(true)
    ->Unit(benchmark::kMillisecond)
    ->Apply(countInput);

BENCHMARK_CAPTURE(
    BM_LoadFile, load_file,
    TSV_PATH,
    // Use to argument type detection
    UnorderedLogFile(TSV_PATH))
    ->Repetitions(5)
    ->ReportAggregatesOnly(true)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
