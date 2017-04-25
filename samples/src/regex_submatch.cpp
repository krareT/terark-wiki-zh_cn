#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/forward_decl.hpp>
#include <terark/fsa/mre_match.hpp>
#include <terark/util/linebuf.hpp>
#include <terark/util/profiling.hpp>
#include <getopt.h>

using namespace terark;

void match_and_print(MultiRegexSubmatch& sub, fstring text) {
	printf("%s ----------\n", text.p);
	int max_match_len = sub.match_utf8(text, ::tolower);
	for(int j = 0; j < (int)sub.fullmatch_regex().size(); ++j) {
		int regex_id = sub.fullmatch_regex()[j];
		int nsub = sub.num_submatch(regex_id);
		for(int k = 0; k < nsub; ++k) {
			fstring str = sub(text.p, regex_id, k);
			printf("j=%d regex_id=%d sub(%d): %.*s\n", j, regex_id, k, str.ilen(), str.data());
		}
	}
	printf("max_match_len=%d: %.*s\n", max_match_len, max_match_len, text.p);
}

int main(int argc, char* argv[]) {
	MultiRegexMatchOptions mrOpt;
	bool verbose = false;
	for (int opt=0; (opt = getopt(argc, argv, "m:D:i:v")) != -1; ) {
		switch (opt) {
		case '?': return 1;
		case 'D': mrOpt.enableDynamicDFA = atoi(optarg) != 0;  break;
		case 'm': mrOpt.regexMetaFilePath = optarg;  break;
		case 'i': mrOpt.dfaFilePath = optarg;  break;
		case 'v': verbose  = true;         break;
		}
	}
	if (mrOpt.regexMetaFilePath.empty()) {
		fprintf(stderr, "-m bin_meta_file is required\n");
		return 1;
	}
	if (mrOpt.dfaFilePath.empty()) {
		fprintf(stderr, "-i dfa_file is required\n");
		return 1;
	}
	mrOpt.load_dfa();
	std::unique_ptr<MultiRegexSubmatch>
				sub(MultiRegexSubmatch::create(mrOpt));
	terark::LineBuf line;
	terark::profiling pf;
	long t0 = pf.now();
	long lineno = 0;
	while (line.getline(stdin) > 0) {
		lineno++;
		line.chomp();
		if (verbose)
			match_and_print(*sub, fstring(line.p, line.n));
		else
			sub->match_utf8(fstring(line.p, line.n), ::tolower);
	}
	long t1 = pf.now();
	printf("time=%f's QPS=%f Latency=%f'us\n"
		, pf.sf(t0,t1)
		, lineno/pf.sf(t0,t1)
		, pf.uf(t0,t1)/lineno
		);
	return 0;
}

