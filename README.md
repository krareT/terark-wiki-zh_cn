# Terark Wiki 中文文档

## 概述

本文档主要对 Terark 公司的相关产品、技术进行综合介绍，包括一些工具集，关键技术点等。

关于每个产品的详细说明和测试报告，请参考每个产品本身的 Github Wiki：

- [TerarkDB](https://github.com/Terark/terark-db/wiki)
  - 基于 RocksDB 的框架，使用 Terark 核心技术实现的存储引擎
- [Mongo on TerarkDB](https://github.com/Terark/mongo-on-terarkdb/wiki)
  - 基于 TerarkDB 存储引擎实现的 MongoDB 版本，通过 mongo-rocks 实现
- [MySQL on TerarkDB](https://github.com/Terark/mysql-on-terarkdb/wiki)
  - 基于 TerarkDB 存储引擎实现的 MySQL 版本，通过 my-rocks 实现


## 目录
- [Terark 核心工具集](tools/tools.md)
  - 使用 Terark 的核心算法生成的各类工具
- [Rank-Select](rankselect/rankselect.md)
  - Rank-Select 是 Succinct Data Structures 的算法基础，我们自己实现了一套高性能的 Rank-Select
