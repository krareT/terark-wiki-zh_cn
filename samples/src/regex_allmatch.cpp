#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/forward_decl.hpp>
#include <terark/fsa/mre_match.hpp>
#include <terark/util/linebuf.hpp>
#include <terark/util/profiling.hpp>
#include <getopt.h>

#if defined(__DARWIN_C_ANSI)
	#define malloc_stats() (void)(0)
#else
	#include <malloc.h>
#endif

#ifdef _MSC_VER
	#define malloc_stats() (void)(0)
#endif

using namespace terark;

int main(int argc, char* argv[]) {
	bool verbose = false;
	MultiRegexMatchOptions mrOpt;
	for (int opt=0; (opt = getopt(argc, argv, "D:i:v")) != -1; ) {
		switch (opt) {
		case '?': return 1;
		case 'D': mrOpt.enableDynamicDFA = atoi(optarg) != 0; break;
		case 'i': mrOpt.dfaFilePath = optarg;       break;
		case 'v': verbose  = true;         break;
		}
	}
	if (mrOpt.dfaFilePath.empty()) {
		fprintf(stderr, "usage: -i dfa_file must be provided!\n");
		return 1;
	}
	mrOpt.load_dfa();
	std::unique_ptr<MultiRegexFullMatch>
				all(MultiRegexFullMatch::create(mrOpt));
	terark::profiling pf;
	long long ts = pf.now();
	all->warm_up();
	long long t0 = pf.now();
	long lineno = 0;
	long matched = 0;
	long sumlen = 0;
	terark::LineBuf line;
	while (line.getline(stdin) > 0) {
		lineno++;
		line.chomp();
		all->match_all(fstring(line), ::tolower);
		if (all->size()) {
			if (verbose) {
				printf("line:%ld:", lineno);
				for(int regexId : *all) {
					printf(" %d", regexId);
				}
				printf("\n");
			}
			matched++;
			sumlen += line.size();
		}
	}
	long long t1 = pf.now();
	printf("time(warm_up)=%f's\n", pf.sf(ts, t0));
	printf("time=%f's lines=%ld matched=%ld QPS=%f Throughput=%f'MiB Latency=%f'us\n"
			, pf.sf(t0,t1)
			, lineno
			, matched
			, lineno/pf.sf(t0,t1)
			, sumlen/pf.uf(t0,t1)
			, pf.uf(t0,t1)/lineno
			);
	malloc_stats();
	return 0;
}

