#include <benchmark/benchmark.h>
#include <string>
#include <utility>

#include "cli.hh"
#include "commands.hh"

#define TSV_PATH "../tests/hn_logs.tsv"


extern CmdBuilder::cmd_mapper CmdBuilder::cmds;

struct NullBuffer : public std::streambuf {
public:
  int overflow(int c) { return c; }
};

static NullBuffer null_buffer;
static std::ostream null_stream(&null_buffer);
static Environment env(null_stream);

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
    CmdBuilder::string_vec args = { "count" };
    for (auto _ : st) {
        CmdBuilder::cmd_ptr cmd = CmdBuilder::cmdBuilder<Count>(env, args);
        (*cmd)();
    }
}

void BM_Top(benchmark::State &st, Environment &env) {
    CmdBuilder::string_vec args(2);
    args[0] = "top";
    for (auto _ : st) {
        args[1] = std::to_string(st.range(0));
        CmdBuilder::cmd_ptr cmd = CmdBuilder::cmdBuilder<Top, int>(env, args);
        (*cmd)();
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
        ->Unit(benchmark::kMillisecond);

BENCHMARK_CAPTURE(
        BM_LoadFile, load_file,
        TSV_PATH,
        // Use to argument type detection
        UnorderedLogFile(TSV_PATH))
        ->Repetitions(5)
        ->ReportAggregatesOnly(true)
        ->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();
