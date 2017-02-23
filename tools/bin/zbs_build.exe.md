# zbs\_build.exe

直接执行 zbs\_build.exe，输出如下帮助信息：
```
Usage: zbs_build.exe Options Input-TXT-File
Options:
  -h Show this help information
  -c checksumLevel: default 1, can be 0, 1, 2, 3
     0: checksum disabled
     1: checksum file header
     2: checksum each record(needs extra 4 bytes per record)
     3: checksum zip data area, and do not checksum each record
  -C Check for correctness
  -d Use Dawg String Pool
  -e EntropyAlgo: Use EntropyAlgo for entropy zip, default none
     h: huffman
     f: FSE (Finite State Entropy)
  -n Nest Level
  -r Random get benchmark
  -o Output-Trie-File
  -g Output-Graphviz-Dot-File
  -b BenchmarkLoop : Run benchmark
  -B Input is binary(bson) data
  -S FloatNumber : Sample ratio of dictionary compression, usually < 0.1, or --
     FloatNumber@<file> : sample from <file>, with sample ratio
     dict:<file> : <file> is the predefined sample dictionary
  -L local_match_opt when using dictionary compression
     h: Local Match by hashing, this is the default
     s: Local Match by suffix array
  -U [optional(0 or 1)] use new Ultra ref encoding, default 1
  -Z compress global dictionary

If Input-TXT-File is omitted, use stdin
Note:
   If -S SampleRatio is specified, Input-TXT-File must be a regular file,
   because in this case DictZipBlobStore need to read input file two passes,
   stdin can be redirected from a regular file, but CAN NOT pass through pipes,
   such as `cat Input-TXT-File | ...`
```

## 简要说明

zbs_build.exe 将每输入文本文件的每一行看做一条记录，行号就是记录ID（从 0 开始）。产生的输出包含两个文件，一个是 -o 中指定的文件，另一个是在 -o 的文件后加了 `-dict` 后缀的字典文件。通过 API 加载时这两个文件都需要。

| 选项 | 说明 |
-----|-----|
-c | 见上文 usage|
-C | 检查正确性，需要巨大的额外内存，谨慎使用|
-d | 老选项，不要使用 |
-e | 额外使用熵编码，对压缩率提升很小，一般能提升 3% 左右，但对解压速度影响较大，大约慢 30% |
-n | 老选项，不要使用 |
-b | 压完了顺便跑一下 benchmark，默认按记录 id **顺序**读取|
-r | 跑 benchmark 时，按记录 id **随机**读取|
-B | 输入是 BSON 数据，而不是文本|
-S | 见上文 usage |
-L | 在局部压缩中使用后缀数组，会提高一点压缩率，但会大幅降低压缩速度，对解压速度无任何影响，<br>单条数据较大时压缩率提高比较明显|
-U | zbs 老版本中使用了简单的编码（因为实现简单），新版中使用了复杂的编码，会提高压缩率，基本上对压缩速度和解压都没有影响|
-Z | 把全局字典也压缩一下|
