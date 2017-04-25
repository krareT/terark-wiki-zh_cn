#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <terark/fsa/base_ac.hpp>
#include <terark/util/autoclose.hpp>
#include <terark/util/linebuf.hpp>
#include <getopt.h>

using namespace terark;

struct OnHit {
	void operator()(size_t endpos, const uint32_t* words, size_t cnt, size_t state) const {
		for (size_t i = 0; i < cnt; ++i) {
			if (ac->has_word_length()) {
				int wlen = ac->wlen(words[i]);
				size_t pos = endpos - wlen;
#ifndef NDEBUG
				try {
					std::string acWord = ac->restore_word(state, words[i]);
					assert(fstring(text + pos, wlen) == acWord);
				}
				catch (const std::invalid_argument&) {
					// not a DoubleArray AC automaton
				}
#endif
				printf("hit_endpos=%04d : word_id=%06d : %.*s\n", int(endpos), words[i], wlen, text + pos);
			}
			else {
				printf("hit_endpos=%04d : word_id=%06d\n", int(endpos), words[i]);
				(void)(state); // remove compiler warning
			}
		}
	}
	const BaseAC* ac;
	const char* text;
};
int main(int argc, char* argv[]) {
	const char* dfa_file = NULL;
	const char* txt_file = NULL;
	for (int opt=0; (opt = getopt(argc, argv, "i:f:")) != -1; ) {
		switch (opt) {
			case '?': return 3;
			case 'i': dfa_file = optarg; break;
			case 'f': txt_file = optarg; break;
		}
	}
	if (NULL == dfa_file) {
		fprintf(stderr, "usage: %s -i input_ac_dfa_file [-f text_file_to_be_matched]\n", argv[0]);
		return 1;
	}
	std::unique_ptr<BaseDFA> dfa(BaseDFA::load_from(dfa_file));
	if (dfa->get_ac() == NULL) {
		fprintf(stderr, "Fail: file: %s is not a AC DFA\n", dfa_file);
		return 1;
	}
	terark::Auto_fclose fp;
	if (txt_file) {
		fp = fopen(txt_file, "r");
		if (NULL == fp) {
			fprintf(stderr, "Fail: fopen(%s, r) = %s\n", txt_file, strerror(errno));
			return 1;
		}
	}
	OnHit on_hit = { dfa->get_ac(), NULL };
	if (!on_hit.ac->has_word_length()) {
		fprintf(stderr, "Pattern length was not saved in AC Automata,\n"
			"  -- Only match endpos and pattern_id will be reported!\n");
	}
	terark::LineBuf line;
	while (line.getline(fp.self_or(stdin)) > 0) {
		line.chomp();
		on_hit.text = line.p;
		on_hit.ac->ac_scan(line, ref(on_hit));
	}
	return 0;
}

