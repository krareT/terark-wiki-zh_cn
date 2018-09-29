# zbs\_unzip.exe

该程序位于 [下载 package](http://terark.com/zh/download/tools/latest) 中的 `bin` 目录，要运行该程序，您需要将 [下载 package](http://terark.com/zh/download/tools/latest) 中的 `lib` 目录加入环境变量 `LD_LIBRARY_PATH` 。

## Links

TODO:

## Usage Help
```text
Usage: zbs_unzip.exe Options Input-BlobStore-File [recId1 recId2 ...]
Synopsis:
    If recId1 recId2 ... are provided, just unzip/extract the specified records
    Record id is 0-base, so the min record id is 0, not 1
Options:
    -h Show this help information
    -t Show timing and through put
    -p MMAP_POPULATE(linux) or FileMapping prefetch(windows)
    -r Unzip in random order
    -b Bench mark, this will not output unzipped data
    -B Output as binary, do not append newline for each record
    -T thread num, when benchmark, use multi thread
```
