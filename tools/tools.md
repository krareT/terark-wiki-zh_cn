# Terark核心工具集

[返回首页](../README.md)

## terark 核心工具集

[terark 核心工具集下载](https://www.terark.com/download/tools/latest)，在该下载列表中，核心工具集的名字是
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
[nlt_build.exe](bin/nlt_build.exe.md)|Terark 嵌套Trie树创建(针对 Key )，压缩后的文件可以通过 Terark 的专用 API 加载，并进行搜索<br>[terark-zip-rocksdb](https://github.com/Terark/terark-zip-rocksdb) 的索引(key)使用了该算法<br/>该算法实践了 Terark 提出的 **CO-Index** (**C**ompressed **O**rdered **Index**) 概念|
[zbs_build.exe](bin/zbs_build.exe.md)|Terark 数据库全局压缩(针对 Value )，压缩后的文件可以通过 Terark 的专用 API 加载，并按记录ID随机提取每条记录。[terark-zip-rocksdb](https://github.com/Terark/terark-zip-rocksdb) 的 value 压缩使用了该算法<br/>该算法实践了 Terark 提出的 **PA-Zip**(**P**oint **A**ccessible **Zip**)|
[zbs_unzip.exe](bin/zbs_unzip.exe.md)|全部解压(或定点提取)由 [zbs_build.exe](bin/zbs_build.exe.md) 压缩的数据，也可用作 benchmark|
[fplcat.exe](bin/fplcat.exe.md)|将多个文件打包在一起，以便使用 [zbs_build.exe](bin/zbs_build.exe.md) 进行压缩，打包出来的文件传递给 [zbs_build.exe](bin/zbs_build.exe.md) 时，需要指定 -B 参数|
|[adfa_build.exe](bin/adfa_build.exe.md)|从输入的 Key 集合创建 ADFA(Acyclic DFA: 无环DFA)，输入文本文件，每行一个 Key，<br/>生成的 DFA 可以进行 Key 匹配（全匹配、前缀匹配），也可以通过[特殊的方式实现 Map 功能](http://nark.cc/p/?p=172)|
[ac_build.exe](bin/ac_build.exe.md)|从文本形式的 Pattern 集合创建 AC 自动机，创建出来的 AC 自动机文件可以通过过 Terark 的专用 API 加载，并调用各种匹配函数<br/>输入的 Pattern 文件中，每行一个 Pattern，每个 Pattern 作为精确字符串匹配，单线程匹配性能可达每秒数百MB甚至上GB|
[regex_build.exe](bin/regex_build.exe.md)|从正则表达式集合创建多正则自动机，创建出来的多正则自动机文件可以通过过 Terark 的专用 API 加载，并调用各种匹配函数<br/>因为是多正则匹配，功能比 AC 自动机要强得多，但匹配性能比 AC 自动机要低一些|
[pinyin_build.exe](bin/pinyin_build.exe.md)|创建“根据拼音对汉字短语纠错”的自动机，demo 见 [nark.cc](http://nark.cc)<br/>生成的自动机可以通过 Terark 的专用 API 加载，并执行搜索和纠错|


其他命令行工具使用说明，可参见各命令的 usage help，详细说明我们会逐渐在该文档中完善。
