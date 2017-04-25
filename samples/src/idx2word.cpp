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
	size_t num_words = dawg->num_words();
	for (int i = optind; i < argc; ++i) {
		const char* szidx = argv[i];
		size_t idx = strtoul(szidx, NULL, 10);
		typedef long long ll;
		if (idx >= dawg->num_words()) {
			fprintf(stderr, "idx=%lld >= num_words=%lld\n", (ll)idx, (ll)num_words);
		} else {
			// dawg->nth_word(idx) will throw std::out_of_range for larger idx
			std::string word = dawg->nth_word(idx);
			printf("%08lld  %s\n", (ll)idx, word.c_str());
		}
	}
	return 0;
}

