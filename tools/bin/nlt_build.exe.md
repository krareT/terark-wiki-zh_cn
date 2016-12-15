# nlt\_build.exe

直接执行 nlt\_build.exe，输出如下帮助信息：
```
Usage: %s Options Input-TXT-File
Options:
    -h Show this help information
    -M maxFragLen
    -n Nest Level
    -o Output-Trie-File
    -g Output-Graphviz-Dot-File
    -b BenchmarkLoop : Run benchmark
    -s indicate that input is sorted, the top level sort will be omitted
    -B Input is binary(bson) data
    -T TmpDir, if specified, will use less memory
       TmpLevel@TmpDir, TmpLevel is 0-9
If Input-TXT-File is omitted, use stdin
```

## 简要说明

输入文件是文本，每行包含一个 key，重复 key 在输出的 trie 树中会被消重（但build过程中仍会占据重复的内存）。

如果总共有n个不同的key，生成的 trie 树中，每个key对应一个ID，ID的范围是 `0` 到 `n-1` ，加载 trie 时使用 mmap，API 支持以下操作:

|操作|时间复杂度| 说明 |
------|-------|------|
反向搜索|O(keylen)|通过 ID 得到相应的 key|
正向搜索|O(keylen)| 按 key **精确**搜索（搜索得到相应的ID）|
范围搜索|O(keylen)| 通过 iterator 支持，相当于 std::map<string,...>::lower_bound(key)<br>可正向/反向遍历，相当于 bidirectional_iterator|
前缀搜索|搜索过程 O(prefix\_len)<br>枚举过程 O(sum(result_keylen))|按 key **前缀**搜索，搜索匹配的前缀长度，<br>搜索到以后可枚举匹配该前缀的候选 (key,ID) 集合）|
正则表达式搜索|最快O(regex\_len)<br>最慢O(all\_key\_len*regex\_len)|不同的正则表达式，时间复杂度差异较大<br>正则表达式头部部不确定性越大（例如.*abc），耗时越大|

|||

| 选项 | 说明 |
-----|-----|
-M | 最长片段尺寸，一般不需要指定|
-n | 嵌套层数，最小2，一般不超过5；一般情况下，嵌套越深，压缩率越高，速度越慢|
-o | 输出的嵌套 trie 树文件 |
-b | 如果想运行 benchmark 看一下性能，可以增加该参数，数据尺寸很大时，指定 1 即可|
-g | 输出 graph-viz 的 dot 文件，用于查看树的图形表示|
-s | 如果输入数据已经排序（按ByteArray字典序，`env LC\_ALL=C sort` 命令行的默认排序），<br>指定该参数可以省略嵌套树最外层的排序，提高创建速度，对生成的trie树无任何影响|
-B | 输入是 BSON 数据，而不是文本|
-T | 指定一个临时目录，如果输入数据很大（几十GB），创建过程中可能会消耗较多内存，<br>指定该参数可以把一部分并非必须时刻驻留内存的数据保存在临时文件中 |
