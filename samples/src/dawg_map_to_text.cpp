#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/dawg_map.hpp>
#include <getopt.h>

int main(int argc, char* argv[]) {
	terark::DAWG_Map<int> dm;
	if (argc < 2) {
		fprintf(stderr, "usage: %s dawg_map_file\n", argv[0]);
		return 3;
	}
	dm.load_map(argv[1]);
	for (size_t i = 0; i < dm.size(); ++i) {
		printf("%s\t%d\n", dm.key(i).c_str(), dm.val(i));
	}
	return 0;
}

