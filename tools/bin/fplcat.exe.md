# fplcat.exe

该程序位于 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `bin` 目录，要运行该程序，您需要将 [下载 package](https://terark.com/zh/download/tools/latest) 中的 `lib` 目录加入环境变量 `LD_LIBRARY_PATH` 。

## Usage Help
```text
Usage: fplcat.exe Options [Input-File-List]
  Options:
    -h Show this help information
    -o Output-File
    -v Show verbose info
    -k Save file name as key, default DO NOT save file name
  If Input-File-List is empty, read file names from stdin
```

## Package many many files and compress by zbs_build.exe
```bash
find some/dir | fplcat.exe -o package.pkg
zbs_build.exe -B -S 0.03 -o package.zbs package.pkg
```
