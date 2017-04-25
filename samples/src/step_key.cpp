#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <getopt.h>

using namespace terark;

int delim = '\t';

struct OnWord {
	void operator()(int idx, fstring value) {
		if (strnlen(value.p, value.n) < value.size() || 256 == delim) {
			// value is binary
			printf("%-20.*s idx=%08d bin=", pos, text, idx);
			for (int i = 0; i < value.n; ++i)
				printf("%02X", (byte_t)value.p[i]);
			printf("\n");
		}
		else { // value is text
			printf("%-20.*s idx=%08d val=%.*s\n"
				, pos, text, idx, value.ilen(), value.data());
		}
	}
	int pos;
	const char* text;
};

int main(int argc, char* argv[]) {
	const char* ifile = NULL; // input dfa file name
	bool longest_match = false;
	for (int opt=0; (opt = getopt(argc, argv, "d::i:l")) != -1; ) {
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
		}
	}
	std::unique_ptr<MatchingDFA> dfa;
	if (ifile) dfa.reset(MatchingDFA::load_from(ifile)); // by filename
	else       dfa.reset(MatchingDFA::load_from(stdin)); // by FILE*

	printf("delim=%c(0x%02X)\n", delim, delim);
	for(int i = optind; i < argc; ++i) {
		const char* text = argv[i];
		printf("%s ----------\n", text);
		bool ok;
		MatchContext ctx;
		fstring ftext(text);
		if (longest_match) {
			ok = dfa->step_key_l(ctx, delim, ftext);
		} else {
			ok = dfa->step_key(ctx, delim, ftext);
		}
		printf("%s.ret=%d strlen=%zd ctx[pos=%zd zidx=%zd]: %.*s\n",
			   	longest_match ? "step_key_l" : "step_key",
				ok, ftext.size(), ctx.pos, ctx.zidx, (int)ctx.pos, text);
		if (ok) {
			OnWord on;
			on.text = text;
			on.pos  = int(ctx.pos);
			dfa->for_each_word(ctx.root, ctx.zidx, ref(on));
		}
	}
	return 0;
}

