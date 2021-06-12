# gcc和makefile的简单使用



## 文件说明

|          | windows | linux |
| :------: | :-----: | :---: |
| 目标文件 |  .obj   |  .o   |
|  静态库  |  .lib   |  .a   |
|  动态库  |  .dll   |  .so  |



## 源码生成可执行文件过程

源码文件.c--预处理gcc -E-->预处理后文件.i--编译gcc -S-->汇编文件.s--汇编gcc -c-->目标文件.o--链接ld-->可执行文件



## gcc

以源码hello.c为例来使用gcc

> -std=c99	使用c99标准编译hello.c,-std可以跟c89,c99,gnu89,gnu90,gnu99等,默认使用gnu c,gnu c对标准c有扩展,特别是内核源码中使用了很多gnu c的扩展
>
> ```shell
> gcc -std=c99 hello.c
> ```
>
> -Ox	这是大写字母O,x是数字,使用x等级的优化,如果不加x就是进行最佳优化
>
> ```shell
> gcc -O hello.c
> ```
>
> -o filename	这是小写的字母o,指定输出文件名
>
> ```shell
> gcc hello.c -o hello
> ```
>
> -x c	将后面的输入文件当做c语言源码文件.gcc默认以文件后缀区分文件类型,如果c源码文件不是以.c为后缀,可能会识别错误
>
> ```shell
> mv hello.c hello.x
> gcc -x c hello.x
> ```
>
> -Wall	显示所有告警信息
>
> -Werror	一旦遇到告警就停止编译
>
> -Wno-unused-parameter	忽略-unused-parameter警告
>
> ```shell
> gcc -Wall hello.c -Wno-unused-parameter
> #显示所有警告信息,但是忽略-unused-parameter警告
> ```
>
> 忽略警告还可以在代码中加入:
>
> ```c
> #pragma GCC diagnostic push//推测为保存当前编译器对警告的处理状态
> 
> #pragma GCC diagnostic ignored "-Wxxxx"
> 
> //下面为会产生警告的代码
> 
> ...
> 
> #pragma GCC diagnostic pop//推测为恢复之前保存的编译器对警告的处理状态
> 
> //ignored处可以换成warning,代表开启警告
> 
> //也可以换成error代表升级警告为错误
> 
> ```
>
> -D宏	使用gcc编译时加入宏定义
>
> ```shell
> gcc -D_FILE_OFFSET_BITS=64 hello.c
> #-D后面没空格,将-D后面的字符加入到宏定义中,这里等价于"#definde _FILE_OFFSET_BITS 64"，这里的宏定义相当于在hello.c文件中的第一行加宏定义(在#include前面),如果-D后面的字符没有等于号,相当于将-D后面的字符定义为字符串"1",等价于"#define _FILE_OFFSET_BITS 1"
> #如果把-D换成-U,表示取消通过-D定义的宏（在文件中定义的宏用-U取消无效）
> ```

下面引用内容黏贴于读<<程序员的自我修养>>时记的部分笔记:

> -E	仅做预处理
>
> ```shell
> gcc -E hello.c -o hello.i
> ```
>
> 也可用cpp预处理工具实现
>
> ```shell
> cpp hello.c > hello.i
> ```
>
> -S	仅编译产生汇编代码
>
> ```shell
> gcc -S hello.i -o hello.s
> #会自动预处理源码
> ```
>
> -c	仅汇编产生目标文件
>
> ```shell
> gcc -c hello.c -o hello.o
> #会自动预处理,编译源码
> ```
>
> 也可用as汇编器实现
>
> ```shell
> as hello.s -o hello.o
> ```
>
> -e 函数名	用于指定入口函数(默认为_start)
>
> -Ipath	指定头文件的目录
>
> -Lpath	指定库的搜索目录
>
> -lname	指定库的路径,库前缀lib,后缀.so,.a不用写
>
> -static	使用静态链接文件
>
> -Bdynamic	编译为动态共享文件(默认方式)
>
> --verbose	将编译链接过程的中间步骤打印出来
>
> -fno-common	把所有未初始化的全局变量不以COMMON块的形式处理
>
> -ffunction-sections	将每个函数分别保存到独立段中
>
> -fdata-sections	将每个变量分别保存到独立段中
>
> -fno-builtin	禁止使用gcc编译器的内置函数优化(比如gcc默认会将只使用一个字符串参数的printf替换成puts)
>
> -fno-stack-protector	编译时取消默认的栈检查(gcc7.3默认使用栈检查,在没有加入某些gcc的库的情况下使用手动链接,会提示栈检查的函数没定义)
>
> -m32	64位系统下强制使用32位abi编译
>
> -shared	产生共享对象
>
> -fPIC	产生地址无关代码(在gcc7.3中貌似是默认的)
>
> -Wl,-soname,my_soname	指定输出共享库的SO-NAME为my_soname
>
> -Wl,-rpath my_path	指定链接产生的目标程序的共享库查找路径
>
> -Wl,-export-dynamec	产生可执行文件时,将所有全局符号导出到动态符号表
>
> -Wl,-s	产生输出文件时消除所有符号信息
>
> -Wl,-S	产生输出文件时消除调试符号信息
>
> -fomit-frame-pointer	取消帧指针ebp
>
> -nostartfiles	在链接时不使用标准的启动文件(crt1.o,crti.o,crtn.o)
>
> -nostdlib	在链接时不使用标准的启动文件和库,它包含了-nostartfiles这个参数



gcc后面的选项可以统称为CFLAGS,使用makefile文件时,定义CFLAGS后,makefile中的gcc命令都会默认使用CFLAGS中的选项

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



## makefile

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

```makefile
LIBS = -lm
OBJS = main.o haha.o sin_value.o cos_value.o
CFLAGS = -Wall
main: ${OBJS}
	gcc -o $@ ${OBJS} ${LIBS}
clean:
	rm -f main ${OBJS}
```

make命令的执行流程:

1. makefile中的每个目标都代表了一个文件。
2. 如果只运行make命令，那么默认执行makefile文件中写的第一个目标,一般第一个目标都是all或main。
3. 执行一个目标时，会做两件事情：

   1. 查看与这个目标同名的文件是否存在，如果不存在，那么就运行这个目标下面的命令。
   2. 如果与这个目标同名的文件已经存在，那么就检查与这个目标同名的文件是否需要更新（即目标文件的修改时间是否早于依赖文件的修改时间），如果需要更新，那么就运行这个目标下面的命令。如果目标文件的修改时间晚于依赖文件的时间,就会提示"xxx is up to date".
4. make程序对目标冒号右边的目标文件名的处理是:先在makefile中寻找是否有和目标文件名同名的目标:
   1. 如果有,先执行此目标中的命令.
   2. 如果没有,根据目标文件名找到此目标文件的源程序,执行`gcc -c xxx.o xxx.c`命令编译为目标文件,如果目标文件名不是以.o结尾的,执行`gcc -o xxx xxx.c`而不用在makefile中显式写出.

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
> CFLAGS可以在make命令中定义,比如定义编译参数：`CFLAGS="-Wall" make clean main`,定义宏`make CFLAGS=-DTEST=1`
>
> > CFLAGS使用的优先级顺序：
>>
> > 1.make指令前加的第一
> >
> > 2.makefile文件里定义的第二
> >
> > 3.shell中的变量第三

### makefile复杂语法

#### 判断语句

##### ifeq和ifneq

ifeq判断两个字符串是否相等,ifeq和后面的括号之间必须有空格,如果在目标内,ifeq前不需要加tab键

```makefile
#判断两个字符串是否相等
ifeq ("abc", "abc")
	command
else
	command
endif
#判断VAR变量是否为空
ifeq ($(VAR), )
	command
else
	command
endif
#多条件判断
ifeq ($(VAR), )
	command
else ifeq ($(VAR), "abc")
	command
endif
```

ifneq判断两个字符串是否不相等,用法和ifeq一样

##### ifdef和ifndef

ifdef判断变量是否定义,使用变量时不需要加$符号,用法和ifeq类似

```makefile
ifdef HIDE
	command
else
	command
endif
```

#### makefile中的特殊变量

$@	目标(target)名字

$<	目标依赖文件列表的第一个文件的名字

$^	目标依赖文件列表所有所有文件的名字,如果有重复的名字,会去重

$+	和$^一样,只不过不会去重

$?	目标依赖文件列表中比目标文件新的文件

上面几个符号的例子

```makefile
TARGET          :=arm
all:$(TARGET) start.o hardware.o
	echo $<
	echo $^
```

这里:

$@是all

$<是arm

$^是arm start.o hardware.o



$(subst 要被替换的字符串,用来替换的字符串,被处理的字符串)

用“用来替换的字符串”替换“被处理的字符串”中的“要被替换的字符串”

```makefile
$(subst .c,.o,test1.c test2.c)
```

就会得到test1.o test2.o

$(wildcard 寻找的文件)

在系统中寻找文件

```makefile
$(wildcard *.c)
```

就等于找到系统中所有后缀为.c的文件，返回成以空格隔开的一整行字符

例如：test1.c test2.c test3.c 这样

$(basename 文件名)

取得文件的名字（去掉后缀的意思）

```makefile
$(basename test1.c)
```

就会取得test1

$(filter 过滤器规则 ,需要被搜索的字符串)

在需要被搜索的字符串中提取符合过滤器规则的字符串

```makefile
sources := foo.c bar.c baz.s ugh.h
$(filter %.c ,$(sources))		#输出foo.c bar.c
$(filter %.c %.s,$(sources))	#输出foo.c bar.c baz.s
```

此处SOURCES表示包含.c .cc .cpp等多类型源文件，该过滤器函数将c文件过滤出来，而%.c即为此过滤器规则。

$(strip 变量)

将变量中多个空格合并为一个空格输出

```makefile
test1 = "   a b    c    d"
echo $(test1)	#输出 a b c d
```



#### 赋值符号

=

最基本的赋值,make会将整个makefile展开后,再决定变量的值.也就是说,变量的值将会是整个makefile中最后被指定的值

:=

也是基本的赋值,表示变量的值决定于它在makefile中的位置,而不是整个makefile展开后的最终值

=和:=的区别见下面例子

```shell
x = foo
y = $(x) bar
x = xyz
```

最后y的值是**xyz bar,而不是 foo bar** 

```shell
x := foo
y := $(x) bar
x := xyz
```

y的值将会是**foo bar,而不是 xyz bar了**

?=

如果没有被赋值过就赋予等号后面的值

+=

添加等号后面的值

#### @取消打印

执行make时默认将所有执行的语句打印在屏幕,可以用@符号取消打印命令

@写在命令开头,表示此行命令不打印

```makefile
all:
	echo "555"	#会打印echo 555,也会打印555
	@echo "666"	#不会打印echo 666,会打印666
```

#### .SUFFIXES用法

.SUFFIXES用来说明c源程序如何编译为.o后缀的目标文件.

SUFFIXES后面加c源文件名后缀,下面写此后缀的文件编译为目标文件时需要执行的命令,直接看例子:

```makefile
.SUFFIXES: .cpp .c .g
.cpp.o:
	echo "compile cpp"
	g++ -c $<

.c.o:
	echo "compile c
	gcc -c $<
	
.g.o:
	echo "compile g"
	gcc -x c -c $<	#此处加了-x c选项,表示将后面的输入文件按照c语言源文件而不是gcc根据文件名后缀识别的文件来处理.如果不加此选项,编译非正常c源文件后缀的代码可能出错
```

#### 使用%符号来隐式指定目标文件编译规则

比如:

```makefile
all:test1.o test2.o					#此处目标文件必须是.o后缀的
	gcc -o test test1.o test2.o -g

%.o:%.c
	gcc -o $@ $^ -g
```

此时test1.o和test2.o的编译规则是

```makefile
gcc -o test1.o test1.c -g
gcc -o test2.o test2.c -g
```

但是如果makefile有显示的目标编译规则,则显示的编译规则优先

```makefile
all:test1.o test2.o
	gcc -o test test1.o test2.o

%.o:%.c
	gcc -o $@ $^ -g

test1.o:test1.c
	gcc -o test1.o test1.c

test2.o:test2.c
	gcc -o test2.o test2.c
```

此时test1.o和test2.o的编译规则是

```makefile
gcc -o test1.o test1.c
gcc -o test2.o test2.c
```

#### make命令每次都强制重新编译

执行make命令是加-B参数,强制每次都重新编译每个目标\

或者使用.PHONY

##### .PHONY

make编译目标时会判断目标名所代表的文件是否新于所有依赖的文件,如果新,就不会执行此目标中的命令,.PHONY表示忽略后面目标文件,即每次执行make时都执行此目标后的命令,比如:

```makefile
.PHONY: SimpleSection                                                   
 
CC = gcc 
 
all: SimpleSection

SimpleSection: SimpleSection.o
	$(CC) -o SimpleSection $<

clean:
	rm -rf SimpleSection.o SimpleSection
```





## 静态库和动态库的生成和使用

gcc连接时会优先选择动态库,如果想让gcc连接时强制使用静态库,需要加`-static`选项

### 静态库

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

### 动态库

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



## 参考链接

[Makefile 中:= ?= += =的区别](https://www.cnblogs.com/wanghuaijun/p/8092747.html)




