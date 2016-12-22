# terark-wiki

Terark Inc. 中文名“奇简软件（北京）有限公司”。
Terark 的产品包括：
- [terark-db](https://github.com/Terark/terark-db)
- [terark-zip-rocksdb](https://github.com/Terark/terark-zip-rocksdb)

## terark 核心工具集

[terark 核心工具集下载](http://nark.cc/download/)，在该下载列表中，核心工具集的名字是
- terark-fsa_all-Linux-x86_64-g++-**VERSION**-bmi2-0.tgz : 能在较旧的机器上使用
- terark-fsa_all-Linux-x86_64-g++-**VERSION**-bmi2-1.tgz : 只能在 intel-haswell 或更新的机器上使用

请根据自己的机器，和gcc版本，下载相应的版本。

Terark 核心工具集package解压后的目录结构：<br>
root = pkg/terark-fsa_all-Linux-x86_64-g++-**VERSION**-bmi2-**X**<br>

|目录|说明|
--------|---------|
root/bin| **核心命令行工具** |
root/lib| 动态库 |
root/include| 暂时用不上 |
root/samples| |
root/samples/bin| 有一些示例和性能测试工具|
root/samples/src| 示例代码 |

### 核心命令行工具
为了便于编译和各平台统一，即使在 Linux/Mac 中，这些命令行工具也包含 .exe 后缀名。

|名称|功能描述  |
-----|-----|
[nlt_build.exe](tools/bin/nlt_build.exe.md)|Terark 嵌套Trie树创建(针对 Key )，压缩后的文件可以通过 Terark 的专用 API 加载，并进行搜索<br>[terark-zip-rocksdb](https://github.com/Terark/terark-zip-rocksdb) 的索引(key)使用了该算法|
[zbs_build.exe](tools/bin/zbs_build.exe.md)|Terark 数据库全局压缩(针对 Value )，压缩后的文件可以通过 Terark 的专用 API 加载，并按记录ID随机提取每条记录。[terark-zip-rocksdb](https://github.com/Terark/terark-zip-rocksdb) 的 value 压缩使用了该算法|


其他命令行工具使用说明，可参见各命令的 usage help，详细说明我们会逐渐在该文档中完善。

## Rank Select 性能比较

我们的核心产品使用了 Succinct 数据结构，Succinct 数据结构的核心是 Rank Select， Rank Select 的性能至关重要。
为了保证性能，我们自己实现了 Rank Select，比起最流行的开源实现，我们的性能有巨大优势，以下是性能比较：

![rank_select](https://cdn.rawgit.com/terark/terark-wiki-zh_cn/master/graphs/rank_select.svg)
