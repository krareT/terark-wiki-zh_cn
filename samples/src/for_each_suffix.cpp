#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <getopt.h>
#include <stdio.h>

using namespace terark;

static void on_suffix(size_t nth, fstring suffix) {
	printf("%06zd\t%.*s\n", nth, suffix.ilen(), suffix.data());
}

int main(int argc, char* argv[]) {
	const char* ifile = NULL; // input dfa file name
	for (int opt=0; (opt = getopt(argc, argv, "i:")) != -1; ) {
		switch (opt) {
		case '?': return 3;
		case 'i': ifile = optarg;       break;
		}
	}
	std::unique_ptr<MatchingDFA> dfa;
	if (ifile) dfa.reset(MatchingDFA::load_from(ifile)); // by filename
	else       dfa.reset(MatchingDFA::load_from(stdin)); // by FILE*

	for(int i = optind; i < argc; ++i) {
		const char* exact_prefix = argv[i];
		printf("%s ----------\n", exact_prefix);
		dfa->for_each_suffix(exact_prefix, on_suffix);
	}
	return 0;
}

