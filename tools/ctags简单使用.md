# ctags简单使用

## 1.安装

debian系:

`sudo apt install ctags`

## 2.生成索引文件

在待生成索引文件的目录中执行:

`sudo ctags -R`

完成后,在当前目录生成了一个tags文件

可以使用`-f`选项指定生成的文件名

## 3.查询方法

* 在生成tags文件的目录中执行

   `vim -t xxx`

  xxx代表待查询的宏,函数名,变量)

* 1. 在vim配置文件`~/.vimrc`中添加

     > :set tags=tags绝对路径1
     >
     > :set tags+=tags绝对路径2

  2. 在vim中将光标移动到待查询的字段名上,按`ctrl + ]`,将自动跳转到解释此字段的文件中,或者vim进入命令行模式,输入`:cstag xxx`,按`ctrl + t`或`ctrl + o`返回,支持嵌套

     也可以使用`:ta 字段名`来跳转到定义处

  3. 查看此字段在tags文件中共有多少定义:

     `tselect 字段名`

     或者在待查字段上按下`ctrl + ]`后,在vim中执行:

     `:ts`

     可通过按数字来选择对应的解释内容,如果定义过多,屏幕下方显示"更多",无法按选项,可以按两下"esc"键来退出"更多"显示

     如果有多处定义

     跳到上一个和下一个定义:
  
     `:tp`和`:tn`
  
     跳到前或后count个定义:
  
     `:[count]tprevious`
  
     `:[count]tnext`
  
     跳到第一个和最后一个定义:
  
     `:tfirst`
  
     `:tlast`
  
  4. 查看跳转过程:
  
     `:tags`
  
  5. 在新窗口打开查询的字段,光标不会跳过去
  
     `:ptag 字段名`
  
     或者按
  
     `ctrl + w + }`
  
     关闭预览窗口
  
     `pclose`
  
  6. 在新窗口打开查询的字段,光标会跳过去
  
     `:stag 字段名`
  
     或者按
  
     `ctrl + w + ]`

## 4.c++相关的选项

例子:

```shell
ctags -R --c++-kinds=+px --fields=+iaS --extra=+q .
```

> -R	ctags循环生成子目录的tags 
>
> --c++-kinds=+px	ctags记录c++文件中的函数声明和各种外部和前向声明 
>
> --fields=+iaS	ctags要求描述的信息，其中i表示如果有继承，则标识出父类；a表示如果元素是类成员的话，要标明其调用权限（即是public还是private）；S表示如果是函数，则标识函数的signature。 
>
> --extra=+q	强制要求ctags做如下操作—如果某个语法元素是类的一个成员，ctags默认会给其记录一行，可以要求ctags对同一个语法元素再记一行，这样可以保证在VIM中多个同名函数可以通过路径不同来区分。



