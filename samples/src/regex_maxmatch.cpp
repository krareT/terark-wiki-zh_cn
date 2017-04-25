#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#include <terark/fsa/forward_decl.hpp>
#include <terark/fsa/mre_match.hpp>
#include <terark/util/linebuf.hpp>
#include <terark/util/autoclose.hpp>
#include <terark/util/profiling.hpp>
#include <terark/util/unicode_iterator.hpp>
#include <getopt.h>

#if defined(__DARWIN_C_ANSI)
	#define malloc_stats() (void)(0)
#else
	#include <malloc.h>
#endif

#ifdef _MSC_VER
	#define strcasecmp _stricmp
	#define malloc_stats() (void)(0)
	typedef intptr_t ssize_t;
#endif

using namespace terark;

bool read_one_record(FILE* fp, terark::LineBuf* line, bool binary) {
	if (binary) {
		int32_t offsets[2];
		return line->read_binary_tuple(offsets, 1, fp);
	}
	else {
		bool ret =  line->getline(fp) > 0;
		line->chomp();
		return ret;
	}
}

int match_text(MultiRegexFullMatch& fm, fstring text
			 , bool shortest_match, bool ignore_case)
{
	if (shortest_match) {
		if (ignore_case)
		//	 return fm.shortest_match(text, ::tolower);
			 return fm.shortest_match(text, terark::gtab_ascii_tolower);
		else return fm.shortest_match(text);
	}
	else {
		if (ignore_case)
		//	 return fm.match(text, ::tolower);
			 return fm.match(text, terark::gtab_ascii_tolower);
		else return fm.match(text);
	}
}

MultiRegexFullMatch::PosLen
find_first(MultiRegexFullMatch& fm, fstring text
		 , bool latin1, bool shortest_match, bool ignore_case)
{
	if (latin1) {
		if (shortest_match) {
			if (ignore_case)
				 return fm.shortest_byte_find_first(text, terark::gtab_ascii_tolower);
			else return fm.shortest_byte_find_first(text);
		}
		else {
			if (ignore_case)
				 return fm.byte_find_first(text, terark::gtab_ascii_tolower);
			else return fm.byte_find_first(text);
		}
	}
	else {
		if (shortest_match) {
			if (ignore_case)
				 return fm.shortest_utf8_find_first(text, terark::gtab_ascii_tolower);
			else return fm.shortest_utf8_find_first(text);
		}
		else {
			if (ignore_case)
				 return fm.utf8_find_first(text, terark::gtab_ascii_tolower);
			else return fm.utf8_find_first(text);
		}
	}
}

size_t
find_all(MultiRegexFullMatch& fm, fstring text
	   , bool latin1, bool shortest_match, bool ignore_case)
{
	if (latin1) {
		if (shortest_match) {
			if (ignore_case)
				 return fm.shortest_byte_find_all(text, terark::gtab_ascii_tolower);
			else return fm.shortest_byte_find_all(text);
		}
		else {
			if (ignore_case)
				 return fm.byte_find_all(text, terark::gtab_ascii_tolower);
			else return fm.byte_find_all(text);
		}
	}
	else {
		if (shortest_match) {
			if (ignore_case)
				 return fm.shortest_utf8_find_all(text, terark::gtab_ascii_tolower);
			else return fm.shortest_utf8_find_all(text);
		}
		else {
			if (ignore_case)
				 return fm.utf8_find_all(text, terark::gtab_ascii_tolower);
			else return fm.utf8_find_all(text);
		}
	}
}

int main(int argc, char* argv[]) {
	MultiRegexMatchOptions mrOpt;
	const char* txt_file = NULL;
	bool match_all_text = false;
	bool shortest_match = false;
	bool first_only = false;
	bool latin1 = false;
	bool verbose = false;
	bool binary = false;
	bool ignore_case = false;
	for (int opt=0; (opt = getopt(argc, argv, "asFLD:f:i:vBI")) != -1; ) {
		switch (opt) {
			case '?': return 1;
			case 'a': match_all_text = true; break;
			case 's': shortest_match = true; break;
			case 'F': first_only = true; break;
			case 'L': latin1   = true; break;
			case 'D': mrOpt.enableDynamicDFA = atoi(optarg) != 0; break;
			case 'i': mrOpt.dfaFilePath = optarg; break;
			case 'f': txt_file = optarg; break;
			case 'v': verbose  = true;   break;
			case 'B': binary   = true;   break;
			case 'I': ignore_case = true;break;
		}
	}
	if (mrOpt.dfaFilePath.empty()) {
		fprintf(stderr, "usage: %s -i dfa_file [-f match_file] [-v]\n", argv[0]);
		return 1;
	}
	terark::Auto_fclose fp;
	if (txt_file) {
		fp = fopen(txt_file, "r");
		if (NULL == fp) {
			fprintf(stderr, "FATAL: fopen(%s, r) = %s\n", txt_file, strerror(errno));
			return 1;
		}
	}
	mrOpt.load_dfa();
	std::unique_ptr<MultiRegexFullMatch>
		fmPtr(MultiRegexFullMatch::create(mrOpt));
	MultiRegexFullMatch& fm = *fmPtr;
	terark::profiling pf;
	long long ts = pf.now();
	fm.warm_up();
	terark::LineBuf line;
	long long t0 = pf.now();
	long lineno = 0;
	long sumlen = 0;
	long matched = 0;
	while (read_one_record(fp.self_or(stdin), &line, binary)) {
		lineno++;
		if (match_all_text && first_only) {
			MultiRegexFullMatch::PosLen res =
				find_first(fm, line, latin1, shortest_match, ignore_case);
			if (verbose && res.len) {
				printf("line:%ld: (%d %d :", lineno, res.pos, res.len);
				for (size_t i = 0; i < fm.size(); ++i) {
					printf(" %d", fm[i]);
				}
				printf(")\n");
			}
			if (fm.size())
				matched++;
		}
		else if (match_all_text && !first_only) {
			size_t all_match_size =
				find_all(fm, line, latin1, shortest_match, ignore_case);
			if (verbose && all_match_size) {
				printf("line:%ld:", lineno);
				for(size_t i = 0; i < all_match_size; ) {
					size_t j = i;
					int pos = fm.all_match(i).pos;
					int len = fm.all_match(i).len;
					printf(" (%d %d :", pos, len);
					do {
						printf(" %d", fm.all_match(j).regex_id);
						++j;
					} while (j < all_match_size && pos == fm.all_match(j).pos);
					printf(")");
					i = j;
				}
				printf("\n");
			}
			if (all_match_size)
				matched++;
		}
		else {
			int len = match_text(fm, line, shortest_match, ignore_case);
			if (verbose && fm.size()) {
				printf("line:%ld:len=%d:", lineno, len);
				for (size_t i = 0; i < fm.size(); ++i) {
					printf(" %d", fm[i]);
				}
				printf("\n");
			}
			if (fm.size())
				matched++;
		}
		sumlen += line.n;
	}
	long long t1 = pf.now();
	printf("time(warm_up)=%f's\n", pf.sf(ts, t0));
	printf("time=%f's lines=%ld matched=%ld QPS=%f Throughput=%f'MiB Latency=%f'us\n"
			, pf.sf(t0,t1)
			, lineno
			, matched
			, lineno/pf.sf(t0,t1)
			, sumlen/pf.uf(t0,t1)
			, pf.uf(t0,t1)/lineno
			);
	malloc_stats();
	return 0;
}

