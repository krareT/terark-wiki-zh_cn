# adfa_build.exe

该程序位于 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `bin` 目录，要运行该程序，您需要将 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `lib` 目录加入环境变量 `LD_LIBRARY_PATH` 。

## Links
* [把自动机用作 Key-Value 存储](http://nark.cc/p/?p=172)
* [用自动机表达嵌套的数据](http://nark.cc/p/?p=1085)
* [使用 MapReduce 创建超大巨型自动机](http://nark.cc/p/?p=960)
* [DFA的实现](http://nark.cc/p/?p=163)
* [自动机的一些算法和应用](http://nark.cc/p/?p=161)

## Usage Help
```text
At least one of
        -O xfast_dfa_file or -o small_linear_dfa_file or
        -U LoudsDFA_SE    or -u LoudsDFA_IL           or
        -Q quick_dfa_file or -S small_quick__dfa_file is required

Usage:
   adfa_build.exe Options [Input-TXT-File]

Description:
   Build DFA from Input-TXT-File, If Input-TXT-File is omitted, use stdin.
   This program is named adfa_build is because of the DFA graph is Acyclic.

Options:
   -h : Show this help infomation
   -q : Be quiet, don't print progress info
   -O Large-DFA-File : large, but pretty fast
   -o Small-DFA-File : small, but pretty slow, now deprecated
   -Q Quick-DFA-File : small, and pretty fast
   -S Super-DFA-File : small, a little (smaller and slower) than Quick-DFA
   -U Louds-DFA-File : very small, very slow(~5x slower than Large-DFA)
   -u Louds-DFA-File : same as -U, but use RankSelect_IL
   -s : Notify the program that Input-TXT-File is sorted
        The program will use more efficient(less memory and faster) algorithm
   -d : Key-Value Delimiter
   -D : Decompose tail-cross-edge, only for Louds-DFA-File
   -w dup
      Issue warnings when found duplicate lines, argument must be "dup"
   -m [Save-As-MemMap]: Argment is optional, default is 1
       * If not specified this option or omitted the argument, use default(1)
       * If the argument is 0, it means "DO NOT Save-As-MemMap"
   -z MaxZpathTrieNum:MinZpathLen : default is 5:2
   -c : Check Non-Large-DFA-File for correctness
      Note: Enabled when compiling with CXXFLAGS += "-D WITH_CHECK_LINEAR"
            Enabled = NO, WITH_CHECK_LINEAR is NOT defined

   -l [Small-DFA-State-Bytes]:WalkMethod
      Small-DFA-State-Bytes is deprecated and will be ignored
      When specifying WalkMethod, the preceding colon(:) is required
      WalkMethod is the DFA-Graph WalkMethod when building Non-LargeDFA-File
      WalkMethod should be BFS/DFS/PFS, default is PFS
      WalkMethod has no effect when building Louds-DFA-File, which is always BFS
```

## Description
Option `-U` and `-u` use LOUDS succinct encoding, this will generate very small dfa file,
for `url`, or file path test(`find / | adfa_build.exe ...`), the compression rate may up to 100x.
