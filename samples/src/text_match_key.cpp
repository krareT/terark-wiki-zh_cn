#include <terark/fsa/fsa.hpp>
#include <getopt.h>
// A simple match_key/match_key_l example
int main(int argc, char* argv[]) {
	const char* ifile = NULL; // input dfa file name
	bool longest_match = false;
	for (int opt=0; (opt = getopt(argc, argv, "i:l")) != -1; ) {
		switch (opt) {
		case '?': return 3;
		case 'i': ifile = optarg;       break;
		case 'l': longest_match = true; break;
		}
	}
	using namespace terark;
	std::unique_ptr<MatchingDFA> dfa;
	if (ifile) dfa.reset(MatchingDFA::load_from(ifile)); // by filename
	else       dfa.reset(MatchingDFA::load_from(stdin)); // by FILE*
	for(int i = optind; i < argc; ++i) {
		const char* text = argv[i];
		int keylen = 0;
		int len; ///< max_partial_match_len, could be ignored
		auto on_match = [&](int klen, int idx, fstring value) {
			printf("%-20.*s idx=%08d val=%.*s\n"
				, klen, text, idx, value.ilen(), value.data());
			keylen = klen;
		};
		printf("----text=%s\n", text);
		if (longest_match)
			len = dfa->match_key_l(text, on_match);
		else
			len = dfa->match_key(text, on_match);
		if (keylen != len)
			printf("max_partial_match_len=%d: %.*s\n", len, len, text);
	}
	return 0;
}

