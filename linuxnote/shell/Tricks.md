# Tricks



## 进制转换小技巧

- 可以使用declare,let,(())对变量赋一个非十进制的值,再用echo打印出来
- 可以使用

```shell
echo "obase=8;ibase=10;16"|bc
```

> **一定要先设置obase再设置ibase,否则在ibase不为10的情况下无效!**
>
> **十进制以上的值中的字母一定要用大写的!**
>
> ibase和obase默认为10



## 获取字符串长度

```shell
lzh@lzh-ws:test$ str="abcdefg"
#1.利用${#str}来获取字符串的长度
lzh@lzh-ws:test$ echo ${#str}
7
#2.利用awk的length方法
lzh@lzh-ws:test$ echo ${str}|awk '{print length($0)}'
7
#也可以用length($0)来统计文件中每行的长度
lzh@lzh-ws:test$ awk '{print length($0)}' /etc/passwd
#3.利用awk的NF项来获取字符串长度,-F为分隔符，NF为域的个数，即单行字符串的长度
lzh@lzh-ws:test$ echo $str| awk -F "" '{print NF}'
7
#4.利用wc的-L参数来获取字符串的长度
lzh@lzh-ws:test$ echo $str | wc -L
7
#关于wc -L参数: 
lzh@lzh-ws:test$ cat /etc/passwd | wc -L
98
#对多行文件来说,表示打印最长行的长度!98表示/etc/passwd文件最长行的长度为98
#对单行字符串而言,表示当前行字符串的长度
#5.利用wc的-c参数,结合echo -n参数
lzh@lzh-ws:test$ echo -n $str | wc -c
7
#6.利用expr的length方法
lzh@lzh-ws:test$ expr length $str
7
#7.利用expr的$str : ".*"技巧
lzh@lzh-ws:test$ expr $str : ".*"
7
#.*可代表任意字符,即用任意字符来匹配字符串,结果是匹配到7个,即字符串的长度为7
```



## 获取文本文件行数方法

```shell
#1
lzh@lzh-ws:test$ wc -l test.sh 
16 test.sh
#2
lzh@lzh-ws:test$ awk '{print NR}' test.sh|tail -n1
16
#3
lzh@lzh-ws:test$ awk 'END{print NR}' test.sh
16
#4
lzh@lzh-ws:test$ grep -n "" test.sh|awk -F: '{print $1}'|tail -n1
16
#5
lzh@lzh-ws:test$ sed -n '$=' test.sh
16
```

