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
#declare和let也支持赋值时做整数算术运算,比如
declare -i a=3+3
let b=1+2
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

但是,如果整数部分为0,bc的运算结果是不会显示0的,比如

```shell
echo "scale=2;1/2" |bc
#结果为.50
```

可以使用awk工具进行计算

```shell
echo | awk '{print 1/2}'#因为awk需要一个标准输入,所以前面需要加一个echo
#结果为0.5
#如果使用变量
a=1
b=3
echo | awk "{print $a/$b}"
echo | awk '{print '$a/$b'}'
#awk小数运算默认保留6位小数,如果需要设置保留小数位数,可以使用printf
echo | awk '{printf ("%.2f\n",'$a/$b')}'
```

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
source test.sh
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
| -le  | (less than or equal)n1是否小于等于n2    |

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

1. 无论是在[[]]内还是在[[]]外,都可以使用&&和||,都不能使用-o和-a.而[]可以使用-o和-a,但是只能在[]内使用,在[]内不能使用||和&&,如果要在[]外使用逻辑判断,只能使用&&和||

#### 字符的匹配和比较

1. [[]]可以使用=~符号来进行正则匹配,比如判断第一个字符是否包含第二个字符

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
| $BASH_COMAMND            | 代表当前行执行的完整命令.如果执行`echo $BASH_COMAMND`显示的就是"echo $BASH_COMAMND",除了在trap命令重定义的信号操作命令中可以打印收到信号时的脚本执行命令外,在其他地方使用应该都是打印当前打印的命令 |
| $FUNCNAME                | 当前函数名。$FUNCNAME是一个数组，$FUNCNAME[0]代表当前函数名，如果有嵌套，$FUNCNAME[1]代表上层函数名，$FUNCNAME[@]代表嵌套的所有函数 |
| $BASH_SOURCE             | 当前脚本名，和$FUNCNAME一样是一个数组。和$0区别是，如果使用`source test.sh`，test.sh中的$0是父脚本名。这个变量在非bash中不一定有效 |

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
#在case条件的)前,可使用方括号表示一个连续的范围,如[0-9];可使用竖杠符号"|"表示或
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

### 脚本中的getopts

例子:

```shell
while getopts ":a:bc::" opt
do
	case $opt in
		a)
			echo "opt is $opt, $OPTIND have been matched, arg is $OPTARG"
			;;
		b)
			echo "opt is $opt, $OPTIND have been matched, arg is $OPTARG"
			;;
		c)
			echo "opt is $opt, $OPTIND have been matched, arg is $OPTARG"
			;;
		?)
			echo "unknown option or parameter not specified"
			echo "opt is $opt, $OPTIND have been matched, arg is $OPTARG"
			;;
	esac
done
```



getops后面的字符串:

> 一个字符代表一个选项
>
> 字符后面有`:`号代表此选项后面必须加参数
>
> 字符串最前面有`:`号代表隐藏参数错误信息(当使用了非法选项或选项后必须加参数没有加,命令行下都会报错)

不论是使用了非法选项或选项后必须加参数没有加,getops后面的变量都为'?'

"$OPTARG"代表当前选项后的参数

"$OPTIND"代表getops在命令行中已匹配到的选项和参数个数

命令行中选项和参数会依次匹配getopts后面的字符串,如果某个匹配到某个选项时出错,会终止所有选项的匹配,跳出while循环



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
>
> -s	不打印不包含分隔符的行(有助于去掉注释行)

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

### tee

输出到标准输出的同时将文件重定向到文件

> -a	不使用重定向使用追加到文件
>
> -i	让tee命令忽略中断(tee前面的管道符之前的命令不会忽略)

例子:

```shell
#将ping的结果输出到屏幕同时重定向到output1.txt,output2.txt
ping www.baidu.com | tee output1.txt output2.txt
```

当前用户无法在vim中保存所做的修改时,需要使用root权限保存文件,可以执行

```shell
:w !sudo tee %
```

### find

可以看这一篇博客---[linux find -regex 使用正则表达式](https://www.cnblogs.com/jiangzhaowei/p/5451173.html)

很详细,关键是掌握find命令的处理规则

### trap

trap可以捕获信号并重定义信号处理方式,类似c中的signal函数

格式为:

```shell
trap 'command_list'  signals
```

> signals为信号名字或数字,可以用`kill -l`命令查看,可以在一条命令中增加多个信号
>
> command_list中多条命令用;隔开,也可以包含函数
>
> 如果引号中间为空,即忽略信号
>
> 如果信号重定义了,执行`trap signals`即可恢复脚本对信号的默认操作
>
> 貌似SIGKILL信号无法重定义
>
> 当脚本进入trap重定义的信号处理方式后,如果处理方式中没有执行退出脚本,处理方式执行完后,脚本执行位置回到进入trap重定义的信号处理方式前的地方

一般在脚本中使用是为了:

1. 在脚本收到信号退出时删除脚本创建的临时文件;
2. 在脚本收到信号退出时杀死脚本通过&符号创建的后台子进程(如果没有杀死那些子进程,那些子进程的父进程会变成执行脚本的shell);
3. 生成syslog消息.

### logger

通过syslogd服务将消息写入系统消息文件中,最常见的用法是:

`logger -p priority message`

> priority是消息级别
>
> message是消息内容

查看日志文件可以看/var/log/syslog文件,在ubuntu18.04上可以通过`journalctl -f`查看

### expect

expect是一个自动化交互工具,可以根据调用程序的输出给出对应输入,18年我面试时有人问我如何把一个文件用一个脚本分发给内网一个网段中的一批设备,我当时不知道,现在想想通过expect就能实现

expect语法非常复杂,一些关键指令如下:

> spawn					交互程序开始后面跟命令或者指定程序
> expect					获取匹配信息匹配成功则执行expect后面的程序动作
> send exp_send 	用于发送指定的字符串信息
> exp_continue		在expect中多次匹配就需要用到
> send_user			用来打印输出 相当于shell中的echo
> exit						退出expect脚本
> eof						expect执行结束 退出
> set						定义变量
> puts						输出变量
> set timeout			设置超时时间

下面给出一个简单例子,仿照例子就可以完成一般需求:

```shell
#!/usr/bin/expect
#定义文件解释器是expect
#注意expect脚本中,注释最好单独写一行,在某些命令的同一行加注释可能会出问题

#如果脚本参数小于3,退出脚本
if {$argc < 3} {
    puts "Usage:cmd <host> <username> <password>"
    exit 1
}

#设置超时时间,如果调用程序一直没输出expect期望的字符,就退出此脚本
set timeout 30
#设置host变量值为第一个参数,注意,这里不是argv0
set host [lindex $argv 0]
set username [lindex $argv 1]
set password [lindex $argv 2]
set test "test"

#调用ssh程序
spawn ssh $username@$host
#从上往下每行expect依次执行,匹配expect后面的字符,匹配成功了就对调用的ssh程序输入send后面的字符,这行expect后面加了花括号,里面有两行字符,代表此行可以匹配两次字符
expect {
#这里使用了exp_continue,代表expect匹配完"*yes/no"字符后,继续执行此行expect
"*yes/no" { send "yes\r"; exp_continue }
"*password:" { send "$password\r" }
}

#这里匹配终端提示符$,如果登录的是root用户需要把$改为#
expect "*$*" {send "ls\r"}
expect "*$*" {send "hostname\r"}
expect "*$*" {send "touch xxxxxxxxxxxxxx\r"}
expect "*$*" {send "sleep 1\r"}
#执行完成后保持交互状态，把控制权交给控制台,即ssh程序通过此终端来输入输出
#interact

#手动退出ssh连接的用户.如果不手动退出,其实也会自动退出
expect "*$*" {send "exit\r"}
#如果不加这行,ssh程序退出时向终端的打印的字符不显示
expect eof
```

也可以在shell脚本中内嵌expect脚本,方法如下,此shell脚本完全等价于上面expect脚本

```shell
#!/bin/bash

if [ $# != 3 ]; then
	echo "Usage:cmd <host> <username> <password>"
fi

host=$1
username=$2
password=$3

/usr/bin/expect << EOF

set timeout 30
spawn ssh $username@$host
expect {
"*yes/no" { send "yes\r"; exp_continue }
"*password:" { send "$password\r" }
}

#注意!!!此处$符前加了转移字符\,如果不加会被shell识别成shell中的末尾字符$,导致无法匹配
expect "*\$*" {send "ls\r"}
expect "*\$*" {send "hostname\r"}
expect "*\$*" {send "touch xxxxxxxxxxxxxx\r"}
expect "*\$*" {send "sleep 1\r"}

#在shell内嵌expect时不能保持交互状态,interact命令是无效的,而且如果加了这行,这行后面执行的命令输出可能无法打印到终端
#interact

expect "*\$*" {send "exit\r"}
expect eof

EOF
```



## shell脚本扩展

以下内容大部分在文档《shell.md》有介绍，这里总结一下。

Shell脚本支持七种类型的扩展功能：花括号扩展（brace expansion）、波浪号扩展（tilde expansion）、参数与变量替换（parameter and variableexpansion）、命令替换（command substitution）、算术扩展（arithmeticexpansion）、单词切割（word splitting）和路径替换（pathnameexpansion）。

### 花括号扩展（brace expansion）

不可以使用引号。

可以按照花括号里边的模式创建多种文本字符串，花括号里可以使用逗号分隔的单个字符或两个.号分隔的数字。

```shell
[root@localhost ~]# echo {a,bf,d}
a bf d
[root@localhost ~]# echo {a..z}
a b c d e f g h i j k l m n o p q r s t u v w x y z
[root@localhost ~]# echo {1..9..2}
1 3 5 7 9
[root@localhost ~]# echo t{a,e}p
tap tep
```

可以使用多个花括号，每个花括号产生的值可以组合，类似于数据库的外连接，多个花括号也可以嵌套。

```shell
[root@localhost ~]# echo A{1,2}_{3,4}B
A1_3B A1_4B A2_3B A2_4B
You have mail in /var/spool/mail/root
[root@localhost ~]# echo a{A{1,2},B{3,4}}b
aA1b aA2b aB3b aB4b
```

### 波浪号扩展（tilde expansion）

波浪号在Shell脚本中默认代表当前用户的家目录，我们也可以在波浪号后面跟一个有效的账户登录名称，可以返回特定账户的家目录。但是，注意账户必须是系统中的有效账户。

```shell
[root@localhost ~]# echo ~
/root
[root@localhost ~]# 
[root@localhost ~]# echo ~clog
/home/clog
```

### 参数与变量扩展（parameter and variable expansion）

在Shell脚本中我们会频繁地使用$对变量进行扩展替换，变量字符可以放到花括号中，这样可以防止需要扩展的变量字符与其他不需要扩展的字符混淆。如果$后面是位置变量且多于一个数字，必须使用{}，如$1、${11}、${12}。

如果变量字符串前面使用感叹号（!），可以实现对变量的间接引用，而不是返回变量本身的值。感叹号必须放在花括号里面，且仅能实现对变量的一层间接引用：

```shell
[root@localhost ~]# var1="var2"
[root@localhost ~]# var2="hello"
[root@localhost ~]# echo ${var1}
var2
[root@localhost ~]# echo ${!var1}
hello
```

变量替换操作还可以测试变量是否存在及是否为空，若变量不存在或为空，则可以为变量设置一个默认值：

| 变量设置方式     | str没设定              | str为空字符            | str为非空字符     |
| ---------------- | ---------------------- | ---------------------- | ----------------- |
| var=${str-expr}  | var=expr               | var=                   | var=$str          |
| var=${str:-expr} | var=expr               | var=expr               | var=$str          |
| var=${str+expr}  | var=                   | var=expr               | var=expr          |
| var=${str:+expr} | var=                   | var=                   | var=expr          |
| var=${str=expr}  | str=expr  var=expr     | str不變 var=           | str不變var=$str   |
| var=${str:=expr} | str=expr var=expr      | str=expr  var=expr     | str不變  var=$str |
| var=${str?expr}  | expr輸出至stderr（注） | var=                   | var=$str          |
| var=${str:?expr} | expr輸出至stderr（注） | expr輸出至stderr（注） | var=$str          |

```shell
[root@MCDCAP 123]# echo ${ani:-dogs}
dogs
[root@MCDCAP 123]# echo $ani

[root@MCDCAP 123]# 
[root@MCDCAP 123]# 
[root@MCDCAP 123]# echo ${ani2:=dogs}
dogs
[root@MCDCAP 123]# echo $ani2
dogs
[root@MCDCAP 123]# 
[root@MCDCAP 123]# echo ${an3:?'error'}
-bash: an3: error
[root@MCDCAP 123]# 
[root@MCDCAP 123]# an4=123
[root@MCDCAP 123]# echo ${an4:+lock}
lock
[root@MCDCAP 123]# echo $an4
123
```

字符串切割、掐头去尾具体语法和变量内容的统计与替换

```shell
${变量}
为了把某个变量和后面的字符区分开
如:
dir=aaa
echo ${dir}ectory
如果脚本或命令的实参数大于9,表示第9个参数可以用$9,表示第10个参数时必须用${10}

${#变量名}
获取字符串长度

${!前缀字符*}
查找当前shell环境中以指定前缀字符开头的变量名，变量名之间使用IFS分隔，但加引号时，所有变量会变成一个整体。比如把查找出来的值作为参数传给一个函数，无论查找出来的值有多少个，都算函数的一个参数
${!前缀字符@}
查找当前shell环境中以指定前缀字符开头的变量名，查找出来的每个变量名都是独立的。比如把查找出来的值作为参数传给一个函数，即使加了双引号，每查找出一个，都算一个函数参数

${数组名[*]}和${数组名[@]}都表示数组中所有非空元素，每个元素值用空格分开，如果用双引号把它们括起来，那么区别是：
"${数组名[*]}"	被扩展成一个字符串，每个元素以空格分开
"${数组名[@]}"	被扩展成多个词，每个数组元素都是一个词

${数组变量[下标]}
表示数组变量的一个元素
${#数组变量[下标]}
表示此元素值的长度（字符个数）

${!数组名[*]}
列出数组中的所有下标（从0开始），加了双引号会被当作一个整体
${!数组名[@]}
列出数组中的所有下标（从0开始），加了双引号不会被当作一个整体

${变量#字符}
如果字符和变量开头匹配，返回值为去掉此字符开头的变量（支持通配符），去掉部分是与字符匹配最少的。
${变量##*字符}
去掉变量中从左侧数最后一个与此字符相匹配的字符，去掉部分是与字符匹配最多的。
${变量%字符}
如果字符和变量末尾匹配，返回值为去掉此字符末尾的变量（支持通配符），去掉部分是与字符匹配最少的。
${变量%%字符*}
去掉变量中从右侧数最后一个与此字符相匹配的字符，去掉部分是与字符匹配最多的。
记忆方法：#在$左边，所以从左匹配，%在$右边，所以从右匹配。
${变量/旧字符/新字符}
如果变量中含有符合旧字符,则第一个旧字符串会被新字符串替换
${变量//旧字符/新字符}
如果变量中含有符合旧字符,则所有旧字符串会被新字符串替换
${变量/#旧字符/新字符}
如果字符串变量在开头匹配旧字符,则开头的旧字符串会被新字符串替换
${变量/%旧字符/新字符}
如果字符串变量在末尾匹配旧字符,则末尾的旧字符串会被新字符串替换

${#name[*]}和${#name[@]}
数组元素的个数,只要元素值被赋值过,就算某个元素的值被赋值为空也会计入

${变量名:position}
从变量字符串中的第'position'位置开始抽取字符到字符串变量尾部,字符串的第一个字符为0位置
${变量名:position:length}
从变量字符串中的第'position'位置开始抽取'length'个字符,字符串的第一个字符为0位置

${变量名: -position},${变量名:(-position)},${变量名: (-position)}
从变量字符串中的倒数第'position'位置开始抽取字符到字符串变量尾部,字符串的倒数第一个字符为1位置
同理,可以使用${变量名: -position:length}这种方式

#把变量中第一个字符转换成大写
${变量名^}
#把变量中的所有小写字母,全部替换为大写
${变量名^^}
#把变量中的第一个字符换成小写
${变量名,}
#把变量中的所有大写字母,全部替换为小写
${变量名,,}
```



### 命令替换（command substitution）

我们可以通过$(命令)或者\`命令\`命令实现命令替换，推荐使用$(命令)这种方式，该方式支持嵌套的命令替换。

```shell
echo "系统当前登录人数:$(who|wc -l)"
```



### 算术扩展（arithmetic expansion）

算术替换扩展的格式为$(())，也可以使用$[]的形式，算术扩展只支持整数(没有小数操作)，但是可以执行不同的运算，例如加、减、乘、除、取余、取幂、自增、自减、判断是否相等、判断是否不等。

算术扩展支持嵌套。

```shell
[root@localhost ~]# i=1
[root@localhost ~]# echo $((i++))
1
[root@localhost ~]# echo $((++i))
3
[root@localhost ~]# echo $((20/5))
4
[root@localhost ~]# echo $((3==4))
0
[root@localhost ~]# a=3
[root@localhost ~]# b=4
[root@localhost ~]# echo $((a+b))
7
```



### 进程替换（Process Substitution）

命令替换将一个命令的输出结果返回并且赋值给变量，而进程替换则将进程的返回结果通过命名管道的方式传递给另一个进程。
进程替换的语法格式为：＜(命令)或者＞(命令)。一旦使用了进程替换功能，系统将会在/dev/fd/目录下创建文件描述符文件，通过该文件描述符将进程的输出结果传递给其他进程。

```shell
[root@localhost ~]# who | wc -l
1
[root@localhost ~]# wc -l <(who)
1 /dev/fd/63
[root@localhost ~]# ls /dev/fd/63
ls: cannot access /dev/fd/63: No such file or directory

```

通过匿名管道(|)我们可以将一个命令的输出结果传递给另一个进程作为其输入的内容，上面的示例中who | wc -l的目的就是通过匿名管道统计当前系统登录人数。相同的功能我们还可以使用进程替换的方式来实现。＜(who)会将who命令产生的结果保存到/dev/fd/63这个文件描述符中，并将该文件描述符作为wc -l命令的输入参数，最终wc -l ＜(who)的输出结果说明了在/dev/fd/63这个文件中包含5行内容。需要注意的是，文件描述符是实时动态生成的，所以当进程执行完毕，再使用ls查看该文件描述符时会提示没有该文件。

提取/etc/passwd文件中的账户名称（第一列）和家目录（第六列），再提取/etc/shadow中的密码信息（第二列），最后通过paste命令将数据合并为一个文件信息，paste命令会逐行读取多个文件的内容并将多个文件合并。

```shell
paste <(cut -d: -f1,6 /etc/passwd) <(cut -d: -f2 /etc/shadow)
```

一条命令显示当前目录下所有文件并过滤一部分写到两个文件中

```shell
[root@MCDCAP 123]# touch {a,b,c}.sh
[root@MCDCAP 123]# touch {11,22,33}.conf
[root@MCDCAP 123]# ls
11.conf  22.conf  33.conf  a.sh  b.sh  c.sh
[root@MCDCAP 123]# ls |tee >(grep sh$ >sh.1.log) >(grep conf$ >conf.log)
11.conf
22.conf
33.conf
a.sh
b.sh
conf.log
c.sh
sh.1.log
[root@MCDCAP 123]# cat sh.1.log 
a.sh
b.sh
c.sh
[root@MCDCAP 123]# cat conf.log 
11.conf
22.conf
33.conf
```



### 单词切割（word splitting）

单词切割也叫分词，Shell使用IFS变量进行分词处理，默认使用IFS变量的值作为分隔符，对输入数据进分词处理后再执行命令。如果没有自定义IFS，则默认值为空格、Tab制表符和换行符。



### 路径替换（pathname expansion）

除非使用set -f禁用路径替换，否则Bash会在路径和文件名中搜索*、？和[符号，如果找到了这些符号则进行模式匹配的替换，Shell在处理命令时对路径替换后的路径或文件进行处理。如果使用shopt命令时开启了nocaseglob选项，则Bash在进行模式匹配时不区分大小写，默认是区别大小写的。另外，还可以在使用shopt命令时开启extglob选项，可以让Bash支持扩展通配符。shopt命令的-s选项可以开启特定的Shell属性，-u选项可以关闭特定的Shell属性。





















## 参考链接

[Shell中实现整数自增的几种方法示例](https://www.jb51.net/article/121268.htm)

[在脚本中使用 trap](https://www.ibm.com/developerworks/cn/aix/library/au-usingtraps/index.html#list1)

[Linux expect 介绍和用法](https://www.cnblogs.com/saneri/p/10819348.html)

[expect用法介绍](https://blog.csdn.net/anqixiang/article/details/110181689)

[Linux expect详解](https://www.cnblogs.com/zhuyeshen/p/11731942.html)