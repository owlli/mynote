# 使用perf定位热点函数

## 原理

采样cpu运行在哪个函数上，分析运行的函数占比。比如每秒钟采样10000次，其中5000次运行在a函数上，3000次运行在b函数上，那么a函数占比就是50%，b函数占比就是30%。



## 使用方法

### perf top实时查看

```shell
perf top
```

支持使用`-p`或`-t`选项加进程号或线程号查看指定进程或线程。

## 采集数据再分析

```shell
# 采集方法
perf record --call-graph dwarf -p 进程号
# 或-t加线程号

# 查看方法
perf record --call-graph dwarf -p 进程号
```





