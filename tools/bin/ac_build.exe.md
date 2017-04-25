# ac\_build.exe

该程序位于 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `bin` 目录，要运行该程序，您需要将 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `lib` 目录加入环境变量 `LD_LIBRARY_PATH` 。

## Links
* [AC 自动机的实现](http://nark.cc/p/?p=1453)
* [发掘双数组 Trie (Double Array Trie) 的能力](http://nark.cc/p/?p=1480)

## Usage Help

```text
Usage:
   rls/ac_build.exe Options [Input-Pattern-File]

Description:
   Build AC Automaton from Input-Pattern-File, if Input-Pattern-File is
   omitted, use stdin

Options:
   -O AC-Automata-File : Using general dfa trie as base trie
   -d AC-Automata-File : Using DoubleArray trie as base trie
      BaseAC::restore_word(state, word_id) can be used for
      DoubleArray trie based AC-Automata when "-e 1" was used.
   -F Full-AC-DFA-File
      Patch all fail-and-retry-success link physically, this will produce
      a physical DFA for regex ".*(P1|P2|...|Pn)", a simple stupid state
      transition table for this DFA will be very large. To reduce memory usage,
      a state compression schema is used. Since it is not needed to track the
      fail link to reach a target state, it should be much faster, but in real
      world, this is much slower, maybe caused by poor CPU cache hit rate and
      an additional indirection incured by state compression.
   -e 0, 1 or 2: default is 0
      0: Do not save any pattern data, just save AC automata self
      1: Save word length into AC automata, methods will be enabled:
         * BaseAC::wlen(word_id)
         * BaseAC::restore_word(state, word_id)
      2: Save word length and content into AC automata, methods will be enabled:
         * BaseAC::word(word_id) will be valid

Notes:
   word_id will be set as bytes lexicalgraphical ordinal number.
   word_id maybe named as pattern_id somewhere else.
```
