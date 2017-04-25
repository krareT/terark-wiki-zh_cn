#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/fsa.hpp>
#include <stdio.h>

using namespace terark;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s DFA-File\n", argv[0]);
		return 1;
	}
	std::unique_ptr<BaseDFA> dfa(BaseDFA::load_from(argv[1]));
	if (AcyclicPathDFA* adfa = dynamic_cast<AcyclicPathDFA*>(dfa.get())) {
		adfa->print_output(stdout);
	} else {
		fprintf(stderr, "file: %s is not an AcyclicPathDFA\n", argv[1]);
	}
	return 0;
}

