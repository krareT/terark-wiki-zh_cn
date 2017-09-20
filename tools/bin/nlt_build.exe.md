# nlt\_build.exe

该程序位于 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `bin` 目录，要运行该程序，您需要将 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `lib` 目录加入环境变量 `LD_LIBRARY_PATH` 。

该程序生成一种压缩的字典树，生成的字典树可以通过 API 加载，并执行丰富的搜索功能。压缩，指的是的字典树占用的内存，比起构成字典树的那些 Key 的总字节数，压缩了多少倍。

  * 对于自然语言的词表，压缩率一般可以达到 3 到 5 倍
  * 对于 url 集合，压缩率一般可以超过 10 倍，如果 url 平均长度 60 字节，压缩后，平均每个 url 占用不超过 6 个字节

直接执行 nlt\_build.exe，输出如下帮助信息：
```
Usage: rls/nlt_build.exe Options Input-TXT-File
Options:
    -h Show this help information
    -M maxFragLen
    -n Nest Level
    -o Output-Trie-File
    -g Output-Graphviz-Dot-File
    -b BenchmarkLoop : Run benchmark
    -w BenchmarkLoop : Run benchmark with dict_index
    -s indicate that input is sorted, the top level sort will be omitted
    -B Input is binary(bson) data
    -6 Input is base64 encoded data
    -U StrVecType, can be one of:
         x: SortableStrVec, this is the default
         s:   SortedStrVec, -s must also be specified, for double check
         z: ZoSortedStrVec, -s must also be specified, for double check
         f: FixedLenStrVec
         +--------------------------------------------------------+
         |              |Memory Usage|Var Key Len|Can Be UnSorted?|
         |SortableStrVec|    High    |    Yes    |     Yes        |
         |  SortedStrVec|   Medium   |    Yes    |!Must be Sorted!|
         |ZoSortedStrVec|    Low     |    Yes    |!Must be Sorted!|
         |FixedLenStrVec|    Lowest  |    No     |     Yes        |
         +--------------------------------------------------------+
          ZoSortedStrVec is slower than SortedStrVec(20% ~ 40% slower).
          When using ZoSortedStrVec, you should also use -T 4@/path/to/tmpdir,
          otherwise warning will be issued.
    -T TmpDir, if specified, will use less memory
       TmpLevel@TmpDir, TmpLevel is 0-9
    -R RankSelect implementation, can be:
         se-256
         se-512
         il-256
         m-se-512
         m-il-256
         m-xl-256, this is the default
If Input-TXT-File is omitted, use stdin
```

## 简要说明

输入文件是文本，每行包含一个 key，重复 key 在输出的 trie 树中会被消重（但build过程中仍会占据重复的内存）。

如果总共有 n 个不同的 key，生成的 trie 树中，每个 key 对应一个 ID，ID 的范围是 `0` 到 `n-1` ，加载 trie 时使用 mmap，API 支持以下操作:

|操作|时间复杂度| 说明 |
------|-------|------|
反向搜索|`O(keylen)`| 通过 ID 得到相应的 key|
正向搜索|`O(keylen)`| 通过 key **精确搜索**（得到相应的 ID）|
范围搜索|`O(keylen)`| 通过 iterator 支持，相当于 `std::map<string,...>::lower_bound(key)`<br>可正向/反向遍历，相当于 `bidirectional_iterator`|
前缀搜索|搜索过程 `O(prefix_len)`<br>输出过程 `O(sum(result_keylen))`|按 key **前缀**搜索，搜索匹配的前缀长度，<br>搜索到以后可输出匹配该前缀的候选 (key,ID) 集合）|
正则表达式<br>搜索|最快 `O(regex_len)`<br>最慢 `O(all_key_len*regex_len)`|不同的正则表达式，时间复杂度差异较大<br>正则表达式头部不确定性越大（例如`.*abc`），耗时越大|

## 命令行详解
| 选项 | 说明 |
-----|-----|
-M | 最长片段尺寸，一般不需要指定|
-n | 嵌套层数，最小 2，一般不超过 5；一般情况下，嵌套越深，压缩率越高，速度越慢|
-o | 输出的嵌套 trie 树文件 |
-b | 如果想运行 benchmark 看一下性能，可以增加该参数，数据尺寸很大时，指定 1 即可|
-g | 输出 graph-viz 的 dot 文件，用于查看树的图形表示|
-s | 如果输入数据已经排序（按 ByteArray 字典序，`env LC_ALL=C sort` 命令行的默认排序），<br>指定该参数可以省略嵌套树最外层的排序，提高创建速度，对生成的 trie 树无任何影响|
-B | 输入是 BSON 数据，而不是文本，可用于二进制 Key，不验证 BSON 数据的合法性，只要求按照 BSON 的格式：前 4 个字节是 LittleEndian 的 int32 ，表示数据的长度，特别注意：该长度包含 4 字节的 int32 本身|
-T | 指定一个临时目录，如果输入数据很大（几十 GB），创建过程中可能会消耗较多内存，<br>指定该参数可以把一部分并非必须时刻驻留内存的数据保存在临时文件中 |
-U | StrVecType, 参考 usage
