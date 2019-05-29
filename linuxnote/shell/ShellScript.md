# ShellScript



## 解释器和返回值

1. 第一行指定使用的shell类型,如果没有指定,默认使用当前shell

```shell
#!/bin/bash
```

在后面可以加一些shell选项

> -x	打印脚本中执行的语句
>
> -e	脚本中一旦出现返回值不为0的语句,立即退出

更多shell参数可以看我另一篇<<shell.txt>>的笔记

2. 如果没有定义返回值，默认返回值为脚本中最后一条指令的返回值



## 变量的赋值和运算

### 赋值

1. **shell中用=给变量赋值时,=两边不能有空格**
2. shell中直接用=给变量赋值,变量的类型都是字符串类型,如

```shell
#这里"123","22"都是一个字符串,不是整数,不能直接进行数学计算
i=123
n=22
#如果直接进行数学计算并将值赋值给另一个
```

3. 如果要指定变量的类型为整型,要用declare,let,(())方法,如:

```shell
declare -i test=123
let test=123
((test=123))
#这三种赋值方法都可以使用其他进制,但用echo输出时都是十进制的
#0开头的是8进制,0x开头的是16进制,如
declare -i test=0xff
let test=077
((test=0xff))
#也可以使用BASE#NUMBER方式,BASE为进制,范围为2到64,NUMBER为这个进制下的数值,如:
declare -i test=16#1f
let i=8#11
((i=8#11))
```

4. 如果想将一条指令的结果赋值给某个变量,可以用``或者$(),如:

```shell
name=`id -nu`
#或者
name=$(id -nu)
```

**注意:平时能用$()就用$(),曾经遇到个问题(20190509)**

```shell
cat Android.mk
LOCAL_PREBUILT_JNI_LIBS  := \
include $(BUILD_PREBUILT)

awk '/include\ \$\(BUILD_PREBUILT\)/{print FNR}' Android.mk
#可以看到终端打印了行号,但是
a=`awk '/include\ \$\(BUILD_PREBUILT\)/{print FNR}' Android.mk`
#这时a的值是空,只有用
a=$(awk '/include\ \$\(BUILD_PREBUILT\)/{print FNR}' Android.mk)
#此时a的值才是行号
```

**这个问题的原因未明**

### 运算

1. 如果要对两个变量进行算术运算,可以使用如下方法:

```shell
i=123
n=22
#方法一
sim=$((${i}+${n}))
#此时sim还是字符串数据
#$(())中的变量可以不用$符号,上面运算等价于:sim=$((i+n))
#如果右边做算术运算的变量不可以转换为整数,就当做整数0处理
#方法二
declare -i sim=${i}+${n}
#方法三
sim=$(expr $i + $l)		#+号两边必须要有空格
#或者
sim=`expr $i + $l`		#+号两边必须要有空格
#方法四
sim=$[$i+$n]
#除了上面三种方法,其他方法都是做的字符串的相加,比如
declare -i i=123
declare -i l=2
m=${i}+${l}
#此时m的值为字符串"123+2"
```

2. 如果需要进行小数运算,可以使用bc工具

```shell
sim=`echo "3.15*3" | bc`
```

> 可以用这种方法计算派值,scale后跟的数为小数点精确位,4*a(1)为bc提供的计算π的函数
>
> ```shell
> echo “scale=${num};4*a(1)” | bc -lq
> ```

### 整数自增的几种方法

**注意:这里的有一个坑,如果脚本中使用了bash的e选项,脚本中执行到`((i++))`或`let i++`会自动退出(但是这两条命令的返回值是0),而`((++i))`或`let ++i`不会,原因未明!**

1. 使用declare声明变量,再用变量和整数进行算数运行

```shell
declare -i x=1
x=x+1
#或者
x+=1
```

2. 使用let命令

```shell
i=1
let i+=1
#或者
let i=$i+1
#或者
let i++
#或者
let ++i
```

3. 使用(())

```shell
i=1
((++i))
#或者
((i++))
```

4. 下面的就和上面介绍的两个变量进行算术运算方法相同

```shell
#使用expr命令
i=1
i=`expr $i + 1`
#或者
i=$(expr $i + 1)
```

```shell
#使用$(())
i=1
i=$(($i + 1))
```

```shell
#使用$[]
i=1
i=$[$i + 1]
```



## 脚本的执行方式

1. 在当前shell中执行

```shell
sour test.sh
#或
. test.sh
```

2. 在子shell中执行

```shell
bash test.sh
#在bash后可以加shell选项,比如bash -x test.sh
#或者
./test.sh
```



## 在脚本中进行判断

可以用true和false来代表真和假

### 使用test命令进行判断

* 判断某个文件的的文件类型,如:

```shell
test -e <filename>
#如果filename文件存在返回真,否则返回假
```

| 符号 | 意义                                               |
| ---- | -------------------------------------------------- |
| -e   | 文件(linux万物皆文件,这里的文件包括文件夹)是否存在 |
| -f   | 文件是否存在且为普通文件(ls -l,以-开头的文件)      |
| -d   | 文件是否存在且为文件夹                             |
| -b   | 文件是否存在且为块文件                             |
| -c   | 文件是否存在且为字符设备                           |
| -S   | 文件是否存在且为socket文件                         |
| -p   | 文件是否存在且为管道文件                           |
| -L   | 文件是否存在且为软连接文件                         |

* 判断文件的权限,如test -r filename判断filename是否有读权限,但(root权限常有例外,比如权限为444的文件,root用户依然有写权限,判断-w的结果为ture)

| 符号 | 意义                                                         |
| ---- | ------------------------------------------------------------ |
| -r   | 判断文件是否存在且有读权限                                   |
| -w   | 判断文件是否存在且有写权限                                   |
| -x   | 判断文件是否存在且有执行权限                                 |
| -u   | 判断文件是否存在且有suid属性                                 |
| -g   | 判断文件是否存在且有sgid属性                                 |
| -k   | 判断文件是否存在且有Sticky bit属性                           |
| -s   | 判断文件是否存在且为非空文件.如果为文件不存在返回false,如果为空返回false,如果文件存在且不为空文件返回true |

* 两个文件之间的对比,如test file1 -nt file2

| 符号 | 意义                                                         |
| ---- | ------------------------------------------------------------ |
| -nt  | (newer than)判断file1是否比file2新                           |
| -ot  | (older than)判断file1是否比file2旧                           |
| -ef  | 判断file1与file2是否为同一档案.可用在判断硬链接上,主要意义在判断两个文件是否指向同一inode号 |

* 两个整数之间的判断,如test n1 -eq n2

| 符号 | 意义                                    |
| ---- | --------------------------------------- |
| -eq  | (equal)两数是否相等                     |
| -ne  | (not equal)两数是否不等                 |
| -gt  | (greater than)n1是否大于n2              |
| -lt  | (less than)n1是否小于n2                 |
| -ge  | (greater than or equal)n1是否大于等于n2 |
| -le  | (less than or equal)n1是否小于n2        |

* 对字符串进行判断

| 符号              | 意义                                                         |
| ----------------- | ------------------------------------------------------------ |
| test -z string    | 判断字串是否为空?若 string为空,则为true                      |
| test -n string    | 判断字串是否为非空?若 string为空,则为false.<br />-n可省略.<br />(如果是变量,需要在变量两边加上双引号,因为如果变量是空,就是test -n,判断”-n”这个字符串是否为空,如果不为空就返回真,所以每次都返回真） |
| test str1 == str2 | 判断str1是否等于str2,如果相等,返回true.<br />如果对比的对象有变量,**记得把变量用双引号括起来**,否则如果变量是空,上面语句就会变成"== str2"了.<br />不能匹配通配符 |
| test str1 != str2 | 判断str1是否不等于str2,如果相等,返回false                    |

* 多重条件判断,如test -r filename -a -x filename

| 符号 | 意义                                                         |
| ---- | ------------------------------------------------------------ |
| -a   | (and)两个条件同时成立时才返回真.如test -r file -a -x file,file同时有r和x权限时,才返回真 |
| -o   | (or)两个条件有一个成立时就返回真                             |
| !    | 反,如test ! -x file,当file没有x权限时,返回真                 |

### 使用[]符号替换test命令

```shell
test -z ${name}
#完全等价于
[ -z ${name} ]
```

注意：

1. []符号中每一个字段之间,字段和[]符号之间都要隔有空格
2. 在[]中的变量,建议用””括起来,比如：

```shell
name=”l z h”
[ $name == “l z h” ]
#上面这句会报语法错误,因为变量替换后上面的命令就变成了[ l z h == “l z h” ]
```

3. 在[]中的字符常量,最好以’’或者””括起来
4. 如果在[]中比较字符串,需要用转义字符,且只能用>,<,不能用>=,<=,比如

```shell
[ "abc" \> "abd" ]
#返回值为假,因为c的ascii码小于d
```



### [[]]条件测试表达式

这种判断方式需要注意:

1. 无论是在[[]]内还是在[[]]外,都可以使用&&和||,都不能使用-o和-a.而[]可以使用-o和-a,但是只能在[]内使用,如果要在[]外使用逻辑判断,只能使用&&和||

#### 字符的匹配和比较

1. [[]]可以使用=~符号来判断第一个字符是否包含第二个字符,第二个字符串支持正则表达式,比如

```shell
[[ $1 =~ ^la{3} ]]
#如果$1以"laaa"开头,返回值为真
```

2. [[]]可以使用==符号来判断两个字符串是否相同,第二个字符串支持通配符,比如

```shell
[[ $1 == *abc* ]]
#如果$1中包含abc,返回值为真
```

3. 如果在[[]]中比较字符串,不需要用转义字符,比如

```shell
[[ "abc" > "abd" ]]
#返回值为假,因为c的ascii码小于d
```



## 脚本的预设变量

| 变量                     | 意义                                                         |
| ------------------------ | ------------------------------------------------------------ |
| $0                       | 代表脚本本身                                                 |
| $1                       | 代表脚本执行时第一个参数                                     |
| $2                       | 代表脚本执行时第二个参数                                     |
| ${10}                    | 从第十个参数起必须要用花括号括起来                           |
| ...                      | 以此类推                                                     |
| $#                       | 代表脚本参数的个数                                           |
| $@                       | 代表"$1" "$2" "$3" "$4"...,每个变量是独立的(用双引号括起来)  |
| $*                       | 代表"$1c$2c$3c$4...",其中c为IFS,预设为空格,所以本例中代表"$1 $2 $3 $4..." |
| ${#name[i]}              | 数组第i个元素值的长度                                        |
| ${#name[*]}或${#name[@]} | 数组中元素个数，只要元素值被赋值过，就算某个元素的值被赋值为空也会计入 |
| ${#name}                 | name变量所占字符串长度                                       |
| $SECONDS                 | 脚本执行到这个变量时所执行的时间,单位为秒                    |
| $LINENO                  | 代表在脚本中的行号                                           |

使脚本参数向左移动n位，默认为1

```shell
shift n
```



## 脚本语法

### 脚本中的条件判断语句

看下面例子

```shell
if [ 條件判斷式 ]; then
	當條件判斷式成立時，可以進行的指令工作內容;
fi
```

```shell
if [ 條件判斷式 ]; then
	當條件判斷式成立時，可以進行的指令工作內容;
else
	當條件判斷式不成立時，可以進行的指令工作內容;
fi
```

```shell
if [ 條件判斷式一 ]; then
	當條件判斷式一成立時，可以進行的指令工作內容;
	elif [ 條件判斷式二 ]; then
		當條件判斷式二成立時，可以進行的指令工作內容;
	else
		當條件判斷式一與二均不成立時，可以進行的指令工作內容;
fi
```

```shell
case $变量 in
	"第一个变量内容")
	命令集
	;;
	"第二个变量内容")
	命令集
	;;
	*)
	不包括第一个和第二个变量内容的其他所有条件执行的命令段
	;;
esac
```

### 脚本中的函数

function一定要在脚本中调用此函数命令的前面,即先声明再使用

函数的返回值不能大于255,如果return的数大于255,实际返回值为:大于255的数%256

function也具有预设变量(位置参数)功能

```shell
function 函数名(){
	命令集
}
```

shell脚本中的函数不能像c函数那样把参数形参写到函数名后的括号中,如果想使用传参功能就要使用$1,$2...这种位置参数,在函数中的位置参数会把脚本的位置参数覆盖

例子

```shell
lzh@ws:bigbee_log$ cat test5.sh 
#!/bin/bash

function sum()
{
	result=$(($1 + $2))
	return $result
}
sum $1 $2
echo $?

lzh@ws:bigbee_log$ ./test5.sh 254 1
255
lzh@ws:bigbee_log$ ./test5.sh 254 2
0
lzh@ws:bigbee_log$ ./test5.sh 254 3
1

```





### 脚本中的循环

当型循环

```shell
while [ 判断式 ]
do
	命令集
done
```

直到型循环

```shell
until [ 判断式 ]
do
	命令集
done
```

while循环加重定向的一个例子:

```shell
while read LINE
do
    echo $LINE
done < /tmp/study-git-submodule-dirs
```

for循环

```shell
#将con1的值依次赋值给var,执行下面的命令集,以此类推
for var in con1 con2 con3 ...
do
	命令集
done
```

可以用

```shell
$(seq 1 100)
#或
{1..100}
```

来代替1到100连续的数,可以用$*来代表全部位置参数

如果在某个目录中有大量.c后缀名的文件,可以使用

```shell
for i in *.c
```

来给i赋值

for循环的另一种使用方法

```shell
for (( 初始值;限制值;執行步階 ))
do
	命令集
done
```

### 脚本中的select

select语句用于交互式菜单设计,比较麻烦且不常用

### 脚本中的getopt

### break,continue,exit

break		跳出一层循环

continue	跳过之后的语句，进行下一次循环

exit			立即退出正在执行的shell脚本，可以设定退出值



## 脚本的追踪与检测

使用bash shellscript.sh时可以加参数对脚本进行追踪和检测

参数如下:

> -n	不执行script,仅检查语法问题
>
> -v	执行script前,将脚本内容全部输出到屏幕上
>
> -x	将使用到的script内容输出到屏幕上,仅输出用到的内容不显示注释







## 脚本中常用命令

### sed和awk

见<<正则表达式和通配符.md>>

### seq

 主要有一下三个参数

>  -f	用来格式化输出
>
>  -s	用来指定分隔符号,默认是回车
>
>  -w	输出同宽数列,不足的位数用0补齐

例子:

```shell
lzh@lzh-ws:~$ seq -f "str%03g" 9 11
str009
str010
str011
```

### sort

排序，默认从字符串的开头以字符排序，注意：如果开头是数字，此排序方法和以数字大小排序不同，应该用-n

> -f	忽略大小寫的差異，例如 A 與 a 視為編碼相同
>
> -b	会忽略每一行前面的所有空白部分,从第一个可见字符开始比较
>
> -M	以月份的名字來排序，例如 JAN, DEC 等等的排序方法
>
> -n	使用数值进行排序,默认是使用字符的ascii码排序的,会出现10比2小的情况(因为1的ascii码比2低)
>
> -r	反向排序
>
> -u	就是 uniq ，就是去重
>
> -t	分隔符号，默认是用 [tab] 来分隔
>
> -k n	以n区间 (field) 来进行排序
>
> -c	会检查文件是否已排好序,如果乱序,则输出第一个乱序的行的相关信息,最后返回1
>
> -C	会检查文件是否已排好序,如果乱序,不输出内容,仅返回1
>
> -m	将两个排好序的文件合并成一个排好序的文件,两个文件必须是已经排好序的.-m选项对未排序的文件合并是没有意义的

例子:

```shell
#以:为分割符，以第三个区域和之后所有的字符为标准排序
cat /etc/passwd | sort -t ':' -k 3
#如果仅以第三个区域排序用：
cat /etc/passwd | sort -t ':' -k 3,3
```

### uniq

删除重复的行

**注意**

uniq只会去重相邻的重复行,所以,如果要对整个文件去重,使用前需要用sort排序或者直接用`sort -u`

> -i	忽略大小字母的不同
>
> -c	打印时在最前面统计此行的出现的行数
>
> -d	仅显示重复的行（此处重复的行指相邻行的重复）
>
> -u	仅显示出现一次的行（此处出现一次指和相邻行不重复）

### cut

切割字符串

> -d	选择分割符，和-f一起用
>
> -f n	打印-d分隔符分割后的第n区域
>
> -c n	打印第n个字符，如果多个字符用逗号,隔开，如果要打印之后的的所有字符用n-

### xargs

可以读取标准输出，根据ifs分隔成arguments，用于不支持管道的命令

> -0	如果輸入的 stdin 含有特殊字元，例如 `, \, 空白鍵等等字元時，這個 -0 參數可以將他還原成一般字元。這個參數可以用於特殊狀態喔！
>
> -e	這個是 EOF (end of file) 的意思。後面可以接一個字串，當 xargs 分析到這個字串時，就會停止繼續工作！字符串用单引号'括起来，单引号'和e之间没有空格
>
> -p	在執行每個指令的 argument 時，都會詢問使用者的意思；
>
> -n	後面接次數，每次 command 指令執行時，要使用幾個參數的意思。
>
> 當 xargs 後面沒有接任何的指令時，預設是以 echo 來進行輸出喔！

如果管道后面的命令需要两个参数,上面的选项无法实现,需要使用-i和-I选项,单独拿出例子来介绍

> -i	使用{}代替管道传来的标准输出内容
>
> -I	使用指定的字符代替{}

```shell
#删除a.txt
ls a.txt | xargs -i mv {} b.txt
ls a.txt | xargs -I xxx mv xxx 
```

**但是,使用xargs是非常危险的,因为xargs能隐藏shell返回的错误**,比如在一个脚本中

```shell
set -e
#当前目录下没有a.txt
ls a.txt | xargs cat
#上面语句返回值为0
echo $?
ls a.txt | xargs -i mv {} b.txt
#上面语句返回值为0
echo $?
```

上面脚本并不会出错退出

### tr

支持正则

> -d "string"	删除出现的字符

例子

```shell
#把回车换成空格:
tr "\n" " "
#删除出现的三个字母组成的字符串
tr -d "[a-z]{3}"
```

### eval

可以给变量的值代表的变量赋值

```shell
lzh@lzh-ws:aaa$ test=name
lzh@lzh-ws:aaa$ eval $test=lzh
lzh@lzh-ws:aaa$ echo $name
lzh
```

或者两次转换变量:

```shell
foo=10 x=foo
y='$'$x
echo $y
$foo
eval y='$'$x
echo $y
10
```

### printf

echo无法输出特定格式的字符串,可以用printf

例子:

```shell
lzh@lzh-ws:~$ printf "%s--%d\n" $HOME $?
```

### read

读取来自标准输入的输入

> 各选项用法:
>
> -p 提示语句 变量
> -n 变量字符长度 变量
> -t 输入时间 变量
> -s 变量		键盘输入字符时不显示在显示器上

### expr

可以用来算术运算求值、处理字符串

```shell
lzh@lzh-ws:test$ expr 3 + 3
6
```

两个数字之间必须有空格,如果是*号,需要在前面加转义字符\

求字符串长度,如果是字符串变量也可以${#string}来求

```shell
lzh@lzh-ws:test$ name=lizhiheng
lzh@lzh-ws:test$ expr length $name
9
```

字符匹配索引,查找字符在字符串中第一次出现的位置,匹配不到,返回0.只能匹配字符,不能匹配字符串,如果第三个参数是字符串,会输出这个字符串中在第二个参数中最先匹配到的字符的出现的位置

```shell
lzh@ws:linuxnote$ string="hello world"
lzh@ws:linuxnote$ echo $string
hello world
lzh@ws:linuxnote$ expr index "$string" "l"
3
lzh@ws:linuxnote$ expr index "$string" "lo"
3
#这里是'l'在string中出现的位置
lzh@ws:linuxnote$ expr index "$string" 'wo'
5
#这里是'o'在string中出现的位置,因为'w'的匹配位置靠后
```

子串抽取,从一个字符串中某一位抽取n位字符,字符串第一位为1

```shell
lzh@ws:linuxnote$ echo $string
hello worldlb
lzh@ws:linuxnote$ expr substr "$string" 2 7
ello wo
```

子串抽取,从一个字符串中匹配匹配一段字符,支持正则表达式

```shell
lzh@ws:linuxnote$ string="20190505hello"
lzh@ws:linuxnote$ echo $string
20190505hello
lzh@ws:linuxnote$ expr match "$string" '\([0-9]*\)'
20190505
lzh@ws:linuxnote$ expr match "$string" '.*\(...\)'
llo
lzh@ws:linuxnote$ expr match "$string" '.*\(.....\)'
hello
lzh@ws:linuxnote$ expr match "$string" '.*\([a-z]\{5\}\)'
hello
lzh@ws:linuxnote$ expr "$string" : '\([0-9]*\)'
20190505
lzh@ws:linuxnote$ expr "$string" : '.*\(...\)'
llo
lzh@ws:linuxnote$ expr "$string" : '.*\(.....\)'
hello
lzh@ws:linuxnote$ expr "$string" : '.*\([a-z]\{5\}\)'
hello
```







## 参考链接

[Shell中实现整数自增的几种方法示例](https://www.jb51.net/article/121268.htm)