#include "benchmark.h"
#include "Cmalloc.h"

void custom_assert(int expression, const char *message)
{
  if (!expression)
  {
    puts("Assertion failed:");
    puts(message);
    return ;
  }
}
#define assert(expression) custom_assert(expression, #expression)
#define ULLONG_MAX 0xffffffffffffffff
#define LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ROUNDUP(a, sz) ((((uintptr_t)a) + (sz)-1) & ~((sz)-1))

Benchmark *current;
Setting *setting;

static char *hbrk;

static uint64_t uptime() { return 1000 * time(0); }

static char *format_time(uint64_t us) {
  static char buf[32];
  double ms = us / 1000;
  us -= ms * 1000;
  assert(us < 1000);

  Qdtos(buf, ms, 3);
  int len = Qstrlen(buf) - 4;//修改过sprintf函数

  char *p = &buf[len - 1];
  while (us > 0) {
    *(p --) = '0' + us % 10;
    us /= 10;
  }
  return buf;
}

// The benchmark list

#define ENTRY(_name, _sname, _s, _m, _l, _h, _desc) \
  { .prepare = bench_##_name##_prepare, \
    .run = bench_##_name##_run, \
    .validate = bench_##_name##_validate, \
    .name = _sname, \
    .desc = _desc, \
    .settings = {_s, _m, _l, _h}, },

Benchmark benchmarks[] = {
  BENCHMARK_LIST(ENTRY)
};

// Running a benchmark
static void bench_prepare(Result *res) {
  res->usec = uptime();
}

static void bench_reset() {
  hbrk = (void *)ROUNDUP(HEAP_START, 8);
}

static void bench_done(Result *res) {
  res->usec = uptime() - res->usec;
}

static const char *bench_check(Benchmark *bench) {
  uintptr_t freesp = (uintptr_t)HEAP_END - (uintptr_t)HEAP_START;
  if (freesp < setting->mlim) {
    return "(insufficient memory)";
  }
  return NULL;
}

static void run_once(Benchmark *b, Result *res) {
  bench_reset();       // reset malloc state
  current->prepare();  // call bechmark's prepare function
  bench_prepare(res);  // clean everything, start timer
  current->run();      // run it
  bench_done(res);     // collect results
  res->pass = current->validate();
}

static uint32_t score(Benchmark *b, uint64_t usec) {
  if (usec == 0) return 0;
  return (uint64_t)(REF_SCORE) * setting->ref / usec;
}

int benchmark(int arg, char args[][50])
{
  const char *setting_name = args[1];
  if (arg == 1) {
    puts("Empty mainargs. Use \"ref\" by default\n");//修改过
    setting_name = "ref";
  }
  int setting_id = -1;

  if      (Qstrcmp(setting_name, "test" ) == 0) setting_id = 0;
  else if (Qstrcmp(setting_name, "train") == 0) setting_id = 1;
  else if (Qstrcmp(setting_name, "ref"  ) == 0) setting_id = 2;
  else if (Qstrcmp(setting_name, "huge" ) == 0) setting_id = 3;
  else {
    puts("Invalid mainargs: \"");
    puts(setting_name);
    puts("\"; ");
    puts("must be in {test, train, ref, huge}\n");
    return ;
  }

  puts("======= Running MicroBench [input *");
  puts(setting_name);
  puts("*] =======\n");

  uint32_t bench_score = 0;
  int pass = 1;
  uint64_t t0 = uptime();
  uint64_t score_time = 0;

  for (int i = 0; i < LENGTH(benchmarks); i ++) {
    Benchmark *bench = &benchmarks[i];
    current = bench;
    setting = &bench->settings[setting_id];
    const char *msg = bench_check(bench);
    //printf("[%s] %s: ", bench->name, bench->desc);
    puts("[");
    puts(bench->name);
    puts("] ");
    puts(": ");
    puts(bench->desc);
    if (msg != NULL) {
      //printf("Ignored %s\n", msg);
      puts("Ignored ");
      puts(msg);
      puts("\n");
    } else {
      uint64_t usec = ULLONG_MAX;
      int succ = 1;
      for (int i = 0; i < REPEAT; i ++) {
        Result res;
        run_once(bench, &res);
        //printf(res.pass ? "*" : "X");
        if(res.pass) puts("*");
        else puts("X");
        succ &= res.pass;
        if (res.usec < usec) usec = res.usec;
        score_time += res.usec;
      }

      if (succ) puts(" Passed.");
      else puts(" Failed.");

      pass &= succ;

      uint32_t cur = succ ? score(bench, usec) : 0;

      puts("\n");
      if (setting_id != 0) {
        //printf("  min time: %s ms [%d]\n", format_time(usec), cur);
        puts("  min time: ");
        puts(format_time(usec));
        puts(" ms [");
        char str[32];
        Qitos(str, cur);
        puts(str);
        puts("]\n");
      }

      bench_score += cur;
    }
  }
  uint64_t total_time = uptime() - t0;

  bench_score /= LENGTH(benchmarks);

  //printf("==================================================\n");
  puts("==================================================\n");
  //printf("MicroBench %s", pass ? "PASS" : "FAIL");
  if (pass) puts("MicroBench PASS");
  else puts("MicroBench FAIL");
    if (setting_id >= 2)
    {
      //printf("        %d Marks\n", bench_score);
      puts("        ");
      char str[32];
      Qitos(str,bench_score);
      puts(str);
      puts("Marks\n");
      //printf("                   vs. %d Marks (%s)\n", REF_SCORE, REF_CPU);
      puts("                   vs. ");
      Qitos(str, REF_SCORE);
      puts(str);
      puts(" Marks (");
      puts(REF_CPU);
      puts(REF_CPU);
    } else {
    puts("\n");
  }
  //printf("Scored time: %s ms\n", format_time(score_time));
  puts("Scored time: ");
  puts(format_time(score_time));
  puts(" ms\n");
  // printf("Total  time: %s ms\n", format_time(total_time));
  puts("Total time: ");
  puts(format_time(total_time));
  puts(" ms\n");
  return 0;
}

// Libraries

void* bench_alloc(bench_size_t size) {
  size  = (bench_size_t)ROUNDUP(size, 8);
  char *old = hbrk;
  hbrk += size;
  assert((uintptr_t)HEAP_START <= (uintptr_t)hbrk && (uintptr_t)hbrk < (uintptr_t)HEAP_END);
  for (uint64_t *p = (uint64_t *)old; p != (uint64_t *)hbrk; p ++) {
    *p = 0;
  }
  assert((uintptr_t)hbrk - (uintptr_t)HEAP_START <= setting->mlim);
  return old;
}

void bench_free(void *ptr) {
}

static uint32_t seed = 1;

void bench_srand(uint32_t _seed) {
  seed = _seed & 0x7fff;
}

uint32_t bench_rand() {
  seed = (seed * (uint32_t)214013L + (uint32_t)2531011L);
  return (seed >> 16) & 0x7fff;
}

// FNV hash
uint32_t checksum(void *start, void *end) {
  const uint32_t x = 16777619;
  uint32_t h1 = 2166136261u;
  for (uint8_t *p = (uint8_t*)start; p + 4 < (uint8_t*)end; p += 4) {
    for (int i = 0; i < 4; i ++) {
      h1 = (h1 ^ p[i]) * x;
    }
  }
  int32_t hash = (uint32_t)h1;
  hash += hash << 13;
  hash ^= hash >> 7;
  hash += hash << 3;
  hash ^= hash >> 17;
  hash += hash << 5;
  return hash;
}
