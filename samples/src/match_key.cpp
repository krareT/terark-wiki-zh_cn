#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <getopt.h>

using namespace terark;

int delim = '\t';

struct OnMatch {
	void operator()(int keylen, int idx, fstring value) {
		if (strnlen(value.p, value.n) < value.size() || 256 == delim) {
			// value is binary
			printf("%-20.*s idx=%08d bin=", keylen, text, idx);
			for (int i = 0; i < value.n; ++i)
				printf("%02X", (byte_t)value.p[i]);
			printf("\n");
		}
		else { // value is text
			printf("%-20.*s idx=%08d val=%.*s\n"
				, keylen, text, idx, value.ilen(), value.data());
		}
		this->keylen = keylen;
	}
	const char* text;
	int keylen;
};

int main(int argc, char* argv[]) {
	const char* ifile = NULL; // input dfa file name
	bool longest_match = false;
	auchar_t root_ch = 257;
	for (int opt=0; (opt = getopt(argc, argv, "d::i:lr::")) != -1; ) {
		switch (opt) {
		case '?': return 3;
		case 'd':
			if (optarg)
				delim = optarg[0];
			else // no arg for -d, set delim for binary key-val match
				delim = 256; // dfa built by kvbin_build use 256 as delim
			break;
		case 'i': ifile = optarg;       break;
		case 'l': longest_match = true; break;
		case 'r':
			// set root state as state_move(initial_state, root_ch)
			// currently used for test pinyin_build with edit-distance
			if (optarg)
				root_ch = optarg[0];
			else
				root_ch = 256;
			break;
		}
	}
	std::unique_ptr<MatchingDFA> dfa;
	if (ifile) dfa.reset(MatchingDFA::load_from(ifile)); // by filename
	else       dfa.reset(MatchingDFA::load_from(stdin)); // by FILE*
	MatchContext ctx;
	if (root_ch < 257) {
		ctx.root = dfa->v_state_move(initial_state, root_ch);
	}
	OnMatch on_match;
	for(int i = optind; i < argc; ++i) {
		const char* text = argv[i];
		on_match.text = text;
		on_match.keylen = 0;
		printf("----delim=%c[%02X] text=%s\n", delim, delim, text);
		int len; ///< max_partial_match_len, could be ignored
		if (longest_match)
			len = dfa->match_key_l(ctx, delim, text, ref(on_match));
		else
			len = dfa->match_key(ctx, delim, text, ref(on_match));
		if (on_match.keylen != len)
			printf("max_partial_match_len=%d: %.*s\n", len, len, text);
	}
	return 0;
}

