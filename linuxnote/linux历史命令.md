# linux历史命令



linux历史命令一般保存在~/.bash_histroy中



## history命令常用选项和参数

> n	数字,要列出最近的 n 笔命令列表
> -c	将目前的shell中的所有 history 内容全部消除
> -a	将目前新增的history 指令新增入 histfiles 中，若没有加 histfiles，则预设写入 ~/.bash_history
> -r	将 histfiles 的内容读到目前这个 shell 的 history 记忆中
> -w	将目前的 history 记忆内容写入 histfiles
> -d	后面跟历史记录里的序号，执行后将此序号的历史记录删除



## linux历史命令中的!

> !!	重复上一条命令，也就是“!-1”
> !n	重新执行第n条历史命令
> !-n	重新执行倒数第n条历史命令，如“!-1”就等于“!!”
> !string	重新执行以字符串string开头的最近的历史命令行
> !?string?	重新执行最近的、其中包含字符串string的那条历史命令
> !#	到现在位置所输入的整个命令行
> !$	上一条命令中的最后一个字符（注意：这是屏幕上显示的命令行最后一个字符，和`$_`不同，如执行`alias ll=ls -l`，再执行`ll`，再执行`echo $_`，显示的是`-l`，如果执行的是`echo !$`，则为`ll`，在《变量》文件中也有解释）
> !^	上一条命令的第一个选项或参数（注意：不能是命令，只能是命令后的第一个选项或参数）
> !命令:n	在历史中最后一条此命令的第n个参数，^代表第一个，$代表最后一个



## linux历史命令中实用知识

### 使用 HISTTIMEFORMAT 显示时间戳

export HISTTIMEFORMAT='%F %T '	此变量被设置后，history可看到执行命令的时间戳

### 使用 Ctrl+R 搜索历史

### 快速重复执行上一条命令的4种方法

1. 使用上方向键，并回车执行

2. 按 !! 并回车执行
3. 输入 !-1 并回车执行
4. 按 Ctrl+P 并回车执行

### 使用 HISTSIZE 控制历史命令记录的总行数

```shell
vi ~/.bash_profile
```

HISTSIZE=450	（HISTSIZE 定义了 history 命令输出的记录数，即history命令输出HISSIZE行）
HISTFILESIZE=450	（HISTFILESIZE 定义了在 .bash_history 中保存命令的记录总数，可以理解为.bash_history文件中最多只有HISTFILESIZE行）

### 使用 HISTFILE 更改历史文件名称

```shell
vi ~/.bash_profile
```

HISTFILE=/root/.commandline_warrior	(将历史文件改为/root/.commandline_warrior)

### 使用 HISTCONTROL 从命令历史中剔除连续重复的条目

```shell
export HISTCONTROL=ignoredups
```

### 使用 HISTCONTROL 清除整个命令历史中的重复条目

```shell
export HISTCONTROL=erasedups
```

### 使用 HISTCONTROL 强制 history 不记住特定的命令

```shell
export HISTCONTROL=ignorespace #或 ignoreboth，在命令前面输入一个空格就不会记录历史了
```

### 使用 HISTIGNORE 忽略历史中的特定命令

```shell
export HISTIGNORE=”pwd:ls:who”	#执行完此语句后再执行pwd、ls、who时，在history中查不到
```

### 临时禁用历史功能

```shell
set +o history	#开启禁用历史功能，如果想把这一行在历史记录中也不显示，可以export HISCONTROL=ignorespace
set -o history	#回复记录历史功能
```

### 删除已存在的历史记录

```shell
history -d num	#从历史记录中将此历史记录删除
```

在通过上键查看到自己输入过的历史命令时，可以通过ctrl+u来将此行历史记录删除，不过在history的结果中显示*号



## 另一个查看历史的命令fc

> fc 命令可以编辑曾今执行过的命令，并以批处理的形式让这些命令重新执行
> -l	列出除此命令之前的16个命令
> -n 	不显示编号
> -r 	反向排序
> -e 	文本编辑器名称 指定文本编辑器编辑,默认Vi
> -s 	命令序号 选择命令执行













