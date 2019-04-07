# gcc和makefile的简单使用



## 1.文件说明

|          | windows | linux |
| :------: | :-----: | :---: |
| 目标文件 |  .obj   |  .o   |
|  静态库  |  .lib   |  .a   |
|  动态库  |  .dll   |  .so  |



## 2.源码生成可执行文件过程

源码文件.c--预处理gcc -E-->预处理后文件.i--编译gcc -S-->汇编文件.s--汇编gcc -c-->目标文件.o--链接ld-->可执行文件



## 3.gcc

以源码hello.c为例来使用gcc

```shell
gcc -std=c99 hello.c
#使用c99标准编译hello.c
#-std可以跟c89,c99,gnu89,gnu90,gnu99等
#默认使用gnu c,gnu c对标准c有扩展,特别是内核源码中使用了很多gnu c的扩展
```

```shell
gcc -c hello.c
#仅生成hello.o的目标文件
```

```shell
gcc -O hello.c -c
#这是大写的字母o,仅生成hello.o的目标文件,并进行最佳优化
```

```shell
gcc -o hello hello.c
#这是小写的字母o,后面跟着要生成的二进制可执行程序
```

```shell
gcc -E hello.c
#只做预处理
```

```shell
gcc -S hello.c -o hello.s
#仅预处理和编译，产生.s后缀的汇编代码
```

```shell
gcc -Wall hello.c
#显示所有警告信息
```

```shell
gcc -Werror hello.c
#遇到警告就停止编译
```

```shell
gcc -Wall hello.c -Wno-unused-parameter
#显示所有警告信息,但是忽略-unused-parameter警告
```

忽略警告还可以在代码中加入:

```c
#pragma GCC diagnostic push//推测为保存当前编译器对警告的处理状态

#pragma GCC diagnostic ignored "-Wxxxx"

//下面为会产生警告的代码

...

#pragma GCC diagnostic pop//推测为恢复之前保存的编译器对警告的处理状态

//ignored处可以换成warning,代表开启警告

//也可以换成error代表升级警告为错误

```

```shell
gcc -D_FILE_OFFSET_BITS=64 hello.c
#-D后面没空格,将-D后面的字符加入到宏定义中,这里等价于"#definde _FILE_OFFSET_BITS 64"，这里的宏定义相当于在hello.c文件中的第一行加宏定义(在#include前面),如果-D后面的字符没有等于号,相当于将-D后面的字符定义为字符串"1",等价于"#define _FILE_OFFSET_BITS 1"
#如果把-D换成-U,表示取消通过-D定义的宏（在文件中定义的宏用-U取消无效）
```

```shell
gcc sin.c -lm -L/lib -I/usr/include
#這個指令較常下達在最終連結成 binary file 的時候，
#-lm 指的是 libm.so 或 libm.a 這個函式庫檔案,前缀lib、后缀so和a省略不写；
#-L 後面接的路徑是剛剛上面那個函式庫的搜尋目錄，默认为/lib和/lib64；
#-I 後面接的是原始碼內的 include 檔案之所在目錄，默认为/usr/include。
```

我們通常稱 -Wall 或者 -O 這些非必要的參數為旗標 (FLAGS)，因為我們使用的是 C 程式語言，所以有時候也會簡稱這些旗標為 CFLAGS

默认情况下， GCC在链接时优先使用动态链接库，只有当动态链接库不存在时才考虑使用静态链接库，如果需要的话可以在编译时加上-static选项，强制使用静态链接库。

比如在/usr/dev/mysql/lib目录下有链接时所需要的库文件libmysqlclient.so和libmysqlclient.a，为了让GCC在链接时只用到静态链接库，可以使用下面的命令:

```shell
gcc –L /usr/dev/mysql/lib –static –lmysqlclient test.o –o test
```

静态库链接时搜索路径顺序：

> 1. ld会去找GCC命令中的参数-L
> 2. 再找gcc的环境变量LIBRARY_PATH
> 3. 再找内定目录 /lib /usr/lib /usr/local/lib 这是当初compile gcc时写在程序内的

动态链接时、执行时搜索路径顺序:

> 1. 编译目标代码时指定的动态库搜索路径
> 2. 环境变量LD_LIBRARY_PATH指定的动态库搜索路径
> 3. 配置文件/etc/ld.so.conf中指定的动态库搜索路径
> 4. 默认的动态库搜索路径/lib
> 5. 默认的动态库搜索路径/usr/lib



## 4.makefile

约定俗成,一般开发者使用的makefile文件是大写字母开头的Makefile,使用者使用的makefile是小写的makefile

> 默认的情况下，make命令会在当前目录下按顺序找寻文件名为**“GNUmakefile”、“makefile”、“Makefile”**的文件，找到了解释这个文件。在这三个文件名中，最好使用“Makefile”这个文件名，因为，这个文件名第一个字符为大写，这样有一种显目的感觉。最好不要用“GNUmakefile”，这个文件是GNU的make识别的。有另外一些make只对全小写的“makefile”文件名敏感，但是基本上来说，大多数的make都支持“**makefile”和“Makefile”**这两种默认文件名。
>
> 当然，你可以使用别的文件名来书写Makefile，比如：“Make.Linux”，“Make.Solaris”，“Make.AIX”等，如果要**指定特定的Makefile，你可以使用make的“-f”和“--file”参数**，如：make –f Make.Linux或make --file Make.AIX。

make的作用:

> 简化编译时下达的命令
>
> 如果在编译完成后对源码又修改，再次make仅会对修过的源码进行编译
>
> 最後可以依照相依性來更新 (update) 執行檔(不理解)
>
> **如果同时存在Makefile和makefile,优先执行makefile,一般makefile是给用户用的,Makefile是给开发人员用的**

make基本语法:

```make
目标（target）: 目标文件1 目标文件2 。。。
	（这里必须以tab键开头）gcc -o 想建立的执行文件 目标文件1 目标文件2 。。。
#代表注释
#命令行必须以tab键作为开头
#目标（target）后面必须有冒号:
```

例子:

```make
LIBS = -lm
OBJS = main.o haha.o sin_value.o cos_value.o
CFLAGS = -Wall
main: ${OBJS}
	gcc -o $@ ${OBJS} ${LIBS}
clean:
	rm -f main ${OBJS}
```

make命令的执行流程:

1. makefile中的每个标的都代表了一个文件。

2. 如果只运行make命令，那么默认执行第一个标的。

3. 执行一个标的时，会做两件事情：

   1. 查看与这个标的同名的文件是否存在，如果不存在，那么就运行这个标的下面的命令。

   2. 如果与这个标的同名的文件已经存在，那么就检查与这个标的同名的文件是否需要更新（即标的文件的修改时间是否早于依赖文件的修改时间），如果需要更新，那么就运行这个标的下面的命令。如果标的的修改时间晚于依赖文件的时间,就会提示"xxx is up to date".

注意项:

> 变量与变量内容之间以等号=隔开，等号两边可以有空格，不可以有冒号:
>
> 变量左边不可以有tab键
>
> 习惯上变量最好是大写字母，引用时用${变量}和$(变量)引用
>
> $@代表当前的目标（target）
>
> 命令执行时可以引用变量，gcc命令执行时默认引用CFLAGS这个变量
>
> CFLAGS可以在make命令中定义
>
> 比如：CFLAGS="-Wall" make clean main
>
> > CFLAGS使用的优先级顺序：
> >
> > 1.make指令前加的第一
> >
> > 2.makefile文件里定义的第二
> >
> > 3.shell中的变量第三



## 5.静态库和动态库的生成和使用

gcc连接时会优先选择动态库,如果想让gcc连接时强制使用静态库,需要加`-static`选项

### 1.静态库

待生成的静态库:

libxx.a

xx 指代库名

```shell
#生成静态库
gcc -c yyy.c -o yyy.o
ar -cr libxx.a yyy.o
```

将生成的静态库文件libxx.a放到/usr/local/lib中,将包含静态库的头文件放到/usr/local/include中

编译时执行:

```shell
gcc -L/usr/local/lib  -I/usr/local/include  -o main main.c  -lxx
#-l 参数必须在最后，有依赖(被依赖的库一定写在后方)
```

### 2.动态库

待生成的动态库:

libxx.so

```shell
#生成动态库
gcc -shared -fpic -o libxx.so yyy.c
```

将生成的动态库文件libxx.so放到/usr/local/lib中,将包含静态库的头文件放到/usr/local/include中

在 /etc/ld.so.conf 中添加路径 /usr/local/lib,如果在 /etc/ld.so.conf .d目录中的文件已经添加了此路径,就不需要添加了

```shell
#从动态库路径中重新加载动态库
ldconfig
```

编译时执行:

```shell
gcc -I/usr/local/include -L/usr/local/lib -o main main.c -lxx
```



