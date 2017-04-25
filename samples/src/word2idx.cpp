#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <getopt.h>

using namespace terark;

int main(int argc, char* argv[]) {
	const char* ifile = NULL;
	for (int opt=0; (opt = getopt(argc, argv, "i:")) != -1; ) {
		switch (opt) {
			case '?': return 3;
			case 'i': ifile = optarg; break;
		}
	}
	std::unique_ptr<BaseDFA> dfa;
	if (ifile) dfa.reset(BaseDFA::load_from(ifile)); // by filename
	else       dfa.reset(BaseDFA::load_from(stdin)); // by FILE*
	const BaseDAWG* dawg = dfa->get_dawg();
	if (NULL == dawg) {
		fprintf(stderr, "file \"%s\" is not a dawg\n", ifile?ifile:"stdin");
		return 1;
	}
	for (int i = optind; i < argc; ++i) {
		const char* word = argv[i];
		size_t idx = dawg->index(word);
		if (size_t(-1) == idx)
			printf("NOT FOUND %s\n", word);
		else
			printf("%08zd  %s\n", idx, word);
	}
	return 0;
}

