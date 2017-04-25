#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <terark/util/profiling.hpp>
#include <getopt.h>

using namespace terark;

int main(int argc, char* argv[]) {
	const char* ifile = NULL;
	bool benchmark = false;
	profiling pf;
	for (int opt=0; (opt = getopt(argc, argv, "i:b")) != -1; ) {
		switch (opt) {
			case '?': return 3;
			case 'b': benchmark = true; break;
			case 'i': ifile = optarg; break;
		}
	}
	const char* startKey = NULL;
	if (optind < argc) {
		startKey = argv[optind];
	}
	size_t limit = size_t(-1);
	if (optind + 1 < argc) {
		limit = (size_t)strtoul(argv[optind+1], NULL, 10);
	}
	std::unique_ptr<BaseDFA> dfa;
	if (ifile) dfa.reset(BaseDFA::load_from(ifile)); // by filename
	else       dfa.reset(BaseDFA::load_from(stdin)); // by FILE*
	auto adfa = dynamic_cast<const AcyclicPathDFA*>(dfa.get());
	if (NULL == adfa) {
		fprintf(stderr, "file \"%s\" is not a dawg\n", ifile?ifile:"stdin");
		return 1;
	}
	std::unique_ptr<ADFA_LexIterator> iter(adfa->adfa_make_iter());
	long long t0 = pf.now();
	size_t sumlen = 0;
	size_t count = 0;
	if (startKey ? iter->seek_lower_bound(startKey)
				 : iter->seek_begin()) {
		do {
			fstring w = iter->word();
			sumlen += w.size();
			if (!benchmark) {
				printf("%.*s\n", w.ilen(), w.data());
			}
		} while (++count < limit && iter->incr());
	}
	long long t1 = pf.now();
	if (benchmark) {
		if (count) {
			printf("Bytes      : %9.3f MB\n", sumlen/1e6);
			printf("AvgLen     : %9.3f Bytes\n", sumlen/1.0/count);
			printf("Time       : %9.3f seconds\n", pf.sf(t0,t1));
			printf("Throughput : %9.3f MB/s\n", sumlen/pf.uf(t0,t1));
			printf("QPS        : %9.3f K op/s\n", count/pf.mf(t0,t1));
		}
		else {
			printf("Not Found, no result\n");
		}
	}
	return 0;
}

