#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/dawg_map.hpp>
#include <terark/lcast.hpp>
#include <getopt.h>

using namespace terark;

struct ParseKeyInt {
	void operator()(fstring line, fstring* key, int* val) {
		line.split('\t', &F);
		*key = F[0];
		*val = terark::lcast(F[1]);
	}
	valvec<fstring> F;
};

int main(int argc, char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "usage: %s text_file dawg_dfa_file [binary_out_file]\n", argv[0]);
		return 3;
	}
	if (4 == argc) {
		terark::FileStream text_file(argv[1], "r");
		DAWG_Map<int> dm(BaseDFA::load_from(argv[2]));
		dm.patch_values(text_file, ParseKeyInt());
		dm.save_map(argv[3]);
	} else {
		// 通常情况下，使用这种方式创建 DAWG_Map 更简洁
		// this is the short cut, append values to dawg binary file
		//
		// if dawg file already has values, do overwrite
		// bool overwriteValues = true; // this param is defaulted as true
		DAWG_Map<int> dm;
		dm.complete_dawg_map(argv[1], argv[2], ParseKeyInt() /*, overwriteValues*/);
	}
	return 0;
}

