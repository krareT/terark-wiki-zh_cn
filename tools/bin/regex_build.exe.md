# regex\_build.exe

## Links
* [支持 并、交、差 的正则表达式引擎](http://nark.cc/p/?p=1280)
* [规则引擎建库工具](http://nark.cc/p/?p=177)
* [多正则表达式匹配（Multiple Regular Expression Matching）中的动态 DFA 算法](http://nark.cc/p/?p=178)
* [多正则表达式匹配（Multiple Regular Expression Matching）](http://nark.cc/p/?p=174)
* [有多个初始状态的 DFA](http://nark.cc/p/?p=176)
* [多正则表达式匹配的应用](http://nark.cc/p/?p=1428)
* [一个很难的字符串问题](http://nark.cc/p/?p=153)

## Usage Help

```text
Usage:
   regex_build.exe Options [Input-Regex-File]

Description:
   Compile multiple regular expressions into one DFA

Options:
   -h : Show this help infomation
   -q : Be quiet, don't print progress info
   -I : Ignore case for regex
   -L : Use Latin1 charset, not utf8(the default charset)
   -a Add-DotStar(Optional): can be 0, 1, 2
      0: Do not add DotStar, treat all regular expression as head-anchored
         The result DFA will be a "RegexMatch" DFA, not a "RegexSearch" DFA.
      1: Respect the regex's head anchor
      2: Prepend DotStar on the unioned DFA, this is just for **DEBUG TEST**
      Note:
         * Head anchored regex example: "^Hello\s+World!"
         * If this option is omitted, Add-Dot-Star is 0
         * If the option argument is omitted, Add-Dot-Star is 1
   -O Large-DFA-File: Large, but pretty fast
   -o Small-DFA-File: Small(maybe ~10x smaller than Large-DFA)
      Small-DFA is essentially a kind of Virtual Machine DFA.
      Small-DFA is not only small, but also fast, in most cases, it is as
      faster as Large-DFA. But it may be much faster than Large-DFA in some
      special/ideal cases.
   -b Bin-Meta-File: The meta file used for capturing submatch
   -s Optional-Arg
      Build dfa with submatch capture, but the dfa algorithm can only capture
      one-pass DFA.
      If Optional-Arg is 's', the algorithm will ignore the regex's one-pass
      property, and try to use one-pass capturing algorithm to capture all
      submatches, this may produce bad result. But when using utf8 encoding,
      some unicode-one-pass regex is NOT byte-one-pass, for example:
        "从([^到]+)到([^怎]+)怎么走" is unicode-one-pass but not byte-one-pass.
      By using option "-ss", MultiRegexSubmatch::match_utf8() will successfully
      capture the submatches.
      Prepend a '*' at the start of a line has the same effect of '-ss' for the
      current regex.
   -E C-Program-HeaderFile
      Generate enum constant definitions in C-Program-HeaderFile
          constant name is specified by second column of Input-Regex-File
   -D : Build a dynamic matching DFA
      In many cases, the full unioned dfa can not be built, in this situation,
      dynamic matching DFA is a compromised solution between full-nfa matching
      and full-dfa matching.
      For speed up online dynamic matching, this program use a heuristic algorithm
      to clustering(partial union) SubDFAs offline.
   -z MinZpathLen, default is 2, only effective for full-unioned-dfa
   -g : Write DFA and NFA dot graph file for every regex
   -G : Write dot graph file for unioned DFA
   -c Conflict-Report-File
   -t DFA-Type: can be 'd', '1', '2', default is 'd'
      a: use DFA class which optimized for adfa_build
      d: use DenseDFA
      2: use DenseDFA_V2
   -T Timeout1[:Timeout2]: Timout2 is optional
      Timeout1: Timeout for compiling one regex
      Timeout2: Timeout for union all regex
   -P : DO NOT Limit cluster_union_power_size, default is true

Input-Regex-File format
   This is a tab separated text file, column 1 is the regex, other columns are
   optinal user defined contents.
   The regex is advanced regex: http://terark.cc/p/?p=1280
   Lines can be continued by backslash(\), same as which in C/C++ language.

Bin-Meta-File format
   This is a tab separated text file, column description:
   Column 1: Integer regex-id, start with 0
   Column 2: Integer number-of-submaches(including the full match)
   Column 3: Boolean do-capturing, 1 or 0
   Column 4: Boolean is-one-pass, 1 or 0
   Column 5: String  copy of full Input-Regex-File line
```
