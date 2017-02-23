# fplcat.exe

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
zbs_build.exe -S 0.03 -o package.zbs package.pkg
```
