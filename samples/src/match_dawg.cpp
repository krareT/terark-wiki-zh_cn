#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <getopt.h>

using namespace terark;

const char* func = "match_dawg";
struct OnMatch {
	void operator()(int len, int idx) {
		printf("%s: len=%d idx=%d word=%.*s\n", func, len, idx, len, word);
		max_word_len = len;
	}
	const char* word;
	int max_word_len;
};

int main(int argc, char* argv[]) {
	const char* ifile = NULL;
	int longest_match = 0;
	for (int opt=0; (opt = getopt(argc, argv, "i:lL")) != -1; ) {
		switch (opt) {
			case '?': return 3;
			case 'i': ifile = optarg;    break;
			case 'l': longest_match = 1; break; // same proto as match_dawg
			case 'L': longest_match = 2; break; // easy proto of match_dawg_l
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
	if (longest_match) func = "match_dawg_l";
	for(int i = 1; i < argc; ++i) {
		if (longest_match == 2) { // use easy match_dawg_l
			fstring word = argv[i];
			size_t len, idx;
			if (dawg->match_dawg_l(word, &len, &idx)) {
				printf("match_dawg_l: len=%zd idx=%zd word=%.*s\n", len, idx, int(len), word.p);
			}
		} else {
			OnMatch on_match;
			on_match.word = argv[i];
			int len; ///< max_partial_match_len, could be ignored
			if (longest_match) // same proto as match_dawg
				len = dawg->match_dawg_l(on_match.word, ref(on_match));
			else
				len = dawg->match_dawg(on_match.word, ref(on_match));
			if (len != on_match.max_word_len) {
				printf("%s: max_partial_match_len=%d: %.*s\n", func, len, len, on_match.word);
			}
		}
	}
	return 0;
}

