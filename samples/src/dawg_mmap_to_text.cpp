#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/dawg_map.hpp>
#include <getopt.h>

using namespace terark;

int main(int argc, char* argv[]) {
	if (argc < 3) {
		fprintf(stderr, "usage %s key_file val_file\n", argv[0]);
		return 3;
	}
	const char* key_file = argv[1];
	const char* val_file = argv[2];
	DAWG_Map<int> dm(BaseDFA::load_mmap(key_file));
	dm.load_mmap_values(val_file);
	for (size_t i = 0; i < dm.size(); ++i) {
		printf("%s\t%d\n", dm.key(i).c_str(), dm.val(i));
	}
	return 0;
}

