# patch简单使用



## 生成patch

```shell
diff -Naur <oldfile> <newfile> > <file.patch>
```

> Naur	固定用法,可以man diff查看每个参数意义

## 使用patch

```shell
patch -p<N> < <file.patch>
```

> N代表执行打补丁路径在patch文件中路径所需要取消N条‘/’和其左边部分

举个例子

file.pathch在demo文件夹中,demo文件夹下有old和new两个文件夹,里面的目录树结构如下

```shell
lzh@lzh-ws:demo$ ls
foo.patch  new  old
lzh@lzh-ws:demo$ tree
.
├── foo.patch
├── new
│   └── a
│       └── b
│           └── foo.txt
└── old
    └── a
        └── b
            ├── foo.txt
            ├── foo.txt.orig
            └── foo.txt.rej

6 directories, 5 files
```

foo.patch内容如下:

```shell
lzh@lzh-ws:demo$ cat foo.patch 
diff -Naur old/a/b/foo.txt new/a/b/foo.txt
--- old/a/b/foo.txt	2019-02-21 17:27:37.000199839 +0800
+++ new/a/b/foo.txt	2019-02-21 17:28:34.114568239 +0800
@@ -1,2 +1,2 @@
-old_line_1
-old_line_2
+new_line_1
+new_line_2
```

file.patch文件中路径为old/a/b/foo.txt

如果你在demo目录打补丁,不需要消除"old/a/b/foo.txt"中的任何/和左边的部分,所以N就是0,命令就是

```shell
lzh@lzh-ws:demo$ patch -p0 < foo.patch
```

如果在old目录打补丁,N就是1,命令为

```shell
lzh@lzh-ws:old$ patch -p1 < ../foo.patch
```

如果在a目录打补丁,N就是2,命令为

```shell
lzh@lzh-ws:a$ patch -p2 < ../../foo.patch
```

以此类推,但如果直接使用当前目录下的文件,可以省略p参数

```shell
lzh@lzh-ws:b$ patch < ../../../foo.patch
```

如果一个文件已经打了补丁,再次打补丁会有提示,让源文件撤销之前打的补丁,比如:

```shell
lzh@lzh-ws:demo$ patch -p0 < foo.patch 
patching file old/a/b/foo.txt
Reversed (or previously applied) patch detected!  Assume -R? [n] 
```

默认为n,即不撤销之前打的补丁

如果选择y,就是撤销之前打的补丁,让文件还原

如果你想严格指定是应用补丁可以使用下面命令(就是增加N参数)

```shell
lzh@lzh-ws:demo$ patch -Np0 < foo.patch
```

>  如果文件已经打了补丁,上面命令会执行失败,生成冲突文件foo.txt.rej,并给出提示:
>
> ```shell
> lzh@lzh-ws:demo$ patch -Np0 < foo.patch 
> patching file old/a/b/foo.txt
> Reversed (or previously applied) patch detected!  Skipping patch.
> 1 out of 1 hunk ignored -- saving rejects to file old/a/b/foo.txt.rej
> ```

如果你想严格指定是还原补丁可以使用下面命令(就是增加R参数)

```shell
lzh@lzh-ws:demo$ patch -Rp0 < foo.patch
```

> 如果文件未打补丁,上面命令执行时会给出提示,问是否忽略-R参数,默认为n不忽略
>
> ```shell
> lzh@lzh-ws:demo$ patch -Rp0 < foo.patch 
> patching file old/a/b/foo.txt
> Unreversed patch detected!  Ignore -R? [n]
> ```







