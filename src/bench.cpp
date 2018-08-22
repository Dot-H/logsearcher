#include <benchmark/benchmark.h>
#include <string>
#include <utility>

#include "cli.hh"
#include "commands.hh"
#include "logfile.hh"
#include "logtime.hh"

#define TSV_PATH "../tests/hn_logs.tsv"
#define array_size(a) sizeof(a)

// Coef used to cut to determines parts of the tsv to bench
static double tsv_parts[] = { 0.1, 0.5, 0.8, 1 };

extern CmdBuilder::cmd_mapper CmdBuilder::cmds;

struct NullBuffer : public std::streambuf {
public:
  int overflow(int c) { return c; }
};

static NullBuffer null_buffer;
static std::ostream null_stream(&null_buffer);
static Environment env(null_stream);

/**
 * populate the vector of timerange pointers
 * foreach argument in the range, add the timranges
 */
static void selectTimestamp(benchmark::internal::Benchmark* b) {
    static LogFile f(TSV_PATH);
    std::cerr << "Benchmarking " << f << std::endline;

    MmapGuard mapping(NULL, f.size(), PROT_READ, MAP_SHARED, f.fd(), 0);
    const char *file  = static_cast<const char *>(mapping.addr);
    LogTime *lowerLog = new LogTime(file);

    std::vector<CmdBuilder::timerange *rangeArg> vec(array_size(tsv_parts));
    for (int i = 0; i < array_size(tsv_parts); ++i) {
        int schLog = f.size() * tsv_parts[i];

        LogTime *upperLog = new LogTime(file + f.prevLog(schLog, file));
        CmdBuilder::timerange *rangeArg =
            new std::pair<LogTime, LogTime>(lowerLog, upperLog);

        b->arg(static_cast<int64_t>(rangeArg));
    }
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
        CmdBuilder::cmd_ptr cmd = CmdBuilder::cmdBuilder<Count>(env, args);
        (*cmd)();
    }
}

void BM_Top(benchmark::State &st, Environment &env) {
    for (auto _ : st) {
        Top top(env, st.range(0), );
    }
}

BENCHMARK_CAPTURE(
        BM_Top, top,
        // There are no copy here because of c++17 copy ellision
        create_env(TSV_PATH))
        ->RangeMultiplier(10)
        ->Range(1, 10<<10)
        ->Repetitions(5)
        ->ReportAggregatesOnly(true)
        ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(
        BM_Count, count,
        // There are no copy here because of c++17 copy ellision
        create_env(TSV_PATH))
        ->Repetitions(10)
        ->ReportAggregatesOnly(true)
        ->Unit(benchmark::kMillisecond)
        ->Apply(selectTimestamp);

BENCHMARK_CAPTURE(
        BM_LoadFile, load_file,
        TSV_PATH,
        // Use to argument type detection
        UnorderedLogFile(TSV_PATH))
        ->Repetitions(5)
        ->ReportAggregatesOnly(true)
        ->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();
