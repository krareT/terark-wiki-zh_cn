# terark-wiki

Terark Inc. 中文名“奇简软件（北京）有限公司”。
Terark 的产品包括：
- [https://github.com/Terark/terark-db](terark-db)
- [https://github.com/Terark/terark-zip-rocksdb](terark-zip-rocksdb)

## terark 核心工具集

[http://nark.cc/download/](terark 核心工具集下载)，在该下载列表中，核心工具集的名字是
- terark-fsa_all-Linux-x86_64-g++-**VERSION**-bmi2-0.tgz : 能在较旧的机器上使用
- terark-fsa_all-Linux-x86_64-g++-**VERSION**-bmi2-1.tgz : 只能在 intel-haswell 或更新的机器上使用

请根据自己的机器，和gcc版本，下载相应的版本。

Terark 核心工具集package解压后的目录结构：<br>
root = pkg/terark-fsa_all-Linux-x86_64-g++-**VERSION**-bmi2-**X**<br>

|目录|说明|
--------|---------|
root/bin| **核心命令行工具** |
root/lib| |
root/include| 暂时用不上 |
root/samples| |
root/samples/bin| 有一些示例和性能测试工具|
root/samples/src| |

### 核心命令行工具
为了便于编译和各平台统一，即使在 Linux/Mac 中，这些命令行工具也包含 .exe 后缀名。

|名称(详细描述链接)|功能描述  |
-----|-----|
[tools/bin/nlt_build.exe](nlt_build.exe)|Terark 嵌套Trie树创建(针对 Key )，压缩后的文件可以通过 Terark 的专用 API 加载，并进行搜索|
[tools/bin/zbs_build.exe](zbs_build.exe)|Terark 数据库全局压缩(针对 Value )，压缩后的文件可以通过 Terark 的专用 API 加载，并随机提取每条记录|
