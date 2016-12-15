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
