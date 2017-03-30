# Rank Select

## Rank Select 性能比较

我们的核心产品使用了 Succinct 数据结构，Succinct 数据结构的核心是 Rank Select，所以 Rank Select 的性能至关重要。
为了保证性能，我们自己实现了 Rank Select，比起最流行的开源实现([sdsl-lite](https://github.com/simongog/sdsl-lite))，我们的性能有巨大优势，以下是性能测试结果：

### 测试机器配置

|CPU | |
|------|------|
|CPU 数量 | 2
|CPU 型号 | Xeon E5-2630 v3
|CPU 标称频率|2.4 GHz
|CPU 实际运行频率|2.6 GHz
|CPU 核心数/线程数（单颗）|8核16线程
|CPU 核心数/线程数（总数）|16核32线程
|CPU 缓存尺寸|20M
|CPU bogomips<br>（反应cpu真实速度）|4793

|内存 | |
|------|------|
|内存容量|64GB
|内存频率|DDR4 1866 MHz

### 测试结果

纵坐标是单个操作的耗时，单位是纳秒，横坐标是各种 rank select 实现，`sdsl_v_mcl` 是 [sdsl-lite](https://github.com/simongog/sdsl-lite) 中最快的 rank select 实现。其余的是我们自己的不同实现，包含 `_fast` 后缀的是加了一点额外优化的 inline 函数。

图中文字显示有点问题，比较拥挤，最好点击图片，在浏览器新窗口中打开，会正常显示。

![rank_select](https://cdn.rawgit.com/terark/terark-wiki-zh_cn/master/rankselect/rank_select.svg)
