#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/pinyin_dfa.hpp>
#include <terark/fsa/forward_decl.hpp>
#include <terark/util/linebuf.hpp>
#include <terark/util/profiling.hpp>
#include <getopt.h>

using namespace terark;

bool be_quiet = false;
void OnMatch(int klen, int, fstring value) {
	if (!be_quiet)
		printf("%d\t%.*s\n", klen, value.ilen(), value.data());
}
int main(int argc, char* argv[]) {
	const char* base_pinyin_txt_file = NULL;
	const char* word_pinyin_dfa_file = NULL;
	for (int opt=0; (opt = getopt(argc, argv, "p:qw:")) != -1; ) {
		switch (opt) {
		case 'q': be_quiet = true; break;
		case 'p': base_pinyin_txt_file = optarg; break;
		case 'w': word_pinyin_dfa_file = optarg; break;
		}
	}
	if (!base_pinyin_txt_file || !word_pinyin_dfa_file) {
		fprintf(stderr, 
			"usage: %s -w word_pinyin_dfa_file -p base_pinyin_text_file < input\n",
		   	argv[0]);
		return 1;
	}
	MatchingDFAPtr dfa(MatchingDFA_load(word_pinyin_dfa_file));
	fprintf(stderr, "loaded dfa: %s\n", word_pinyin_dfa_file);
	PinYinDFA_Builder pinyin_dfa_builder(base_pinyin_txt_file);
	fprintf(stderr, "built pinyin_dfa_builder: %s\n", base_pinyin_txt_file);
	terark::LineBuf line;
	terark::profiling pf;
	while (line.getline(stdin) > 0) {
		line.chomp();
		printf("input: %s\n", line.p);
		long long t2 = pf.now();
		MatchingDFAPtr pinyin_dfa(pinyin_dfa_builder.make_pinyin_dfa(line, NULL));
		long long t3 = pf.now();
		if (pinyin_dfa.get() != NULL) {
			PinYinDFA_Builder::match_pinyin(dfa.get(), pinyin_dfa.get(), &OnMatch);
			long long t4 = pf.now();
			printf("time: make_dfa=%f'us match=%f'us\n", pf.uf(t2,t3), pf.uf(t3,t4));
		} else {
			fprintf(stderr, "make_pinyin_dfa failed: %s\n", line.p);
		}
	}
	return 0;
}

