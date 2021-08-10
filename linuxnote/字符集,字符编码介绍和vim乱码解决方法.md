# 字符集,字符编码介绍和vim乱码解决方法



​	本来是遇到vim中文乱码的问题,在网上搜着搜着就找到了一些字符集相关的知识,发现自己一直没弄清编码字符集和字符编码的关系,整理了好久,终于彻底理清了!



[TOC]



## 1.字符集和字符编码

* 字库表(character repertoire):字库表是一个相当于所有可读或者可显示字符的数据库，字库表决定了整个字符集能够展现表示的所有字符的范围

* 编码值(code point):又叫代码点,表示字符在字库表中的位置,例如在ASCII字符集中,字符A被分配成65号,那就是说A的编码值是65
* 编码字符集(coded character set):简称字符集,所有编码值的集合,同一个字符在不同字符集中存储的值不一样,可以搜索字符映射表查到,常见的字符编码集:

>**Unicode**：也叫统一字符集，它包含了几乎世界上所有的已经发现且需要使用的字符（如中文、日文、英文、德文等）
>
>**ASCII**：早期的计算机系统只能处理英文，所以ASCII也就成为了计算机的缺省字符集，包含了英文所需要的所有字符
>
>**GB2312**：中文字符集，包含ASCII字符集。ASCII部分用单字节表示，剩余部分用双字节表示
>
>**GBK**：GB2312的扩展，但完整包含了GB2312的所有内容
>
>**GB18030**：GBK字符集的超集，常叫大汉字字符集，也叫CJK（Chinese，Japanese，Korea）字符集，包含了中、日、韩三国语言中的所有字符
>
>**BIG-5**:台湾使用的繁体字字符集集

* 字符编码(char encoding):是编码字符集和实际存储数值之间的转换关系,代表编码值的二进制存储格式(字符是根据字符编码方案转换为一个二进制数值存储在计算机中的)

>例如在ASCII字符集中，A的编码值是65
>
>在ASCII编码中，这个65被存储为01000001，一共占据一个字节(8个二进制位)
>
>在UTF-8字符编码规范中被存储为8个二进制位:01000001
>
>在UTF-16中被存储为16个二进制位:0000000001000001
>
>而在UTF-32中被存储为32个二进制位:00000000000000000000000001000001

当使用字符编码和字符集时,表达者和理解者都经常把这两个词弄混淆,下面举一些例子帮助理解:

> 编码字符集Unicode，有UTF-8、UTF-16、UTF-32等多种字符编码
>
> 编码字符集ASCII,本身就是编码字符集，又是字符编码
>
> 编码字符集GB2312,只有EUC-CN一种字符编码
>
> **所以在某些情况下,使用GB2312这个词时,使用者可能是想表达的是一套包含了GB2312字符集和EUC-CN字符编码的方案,当使用GBK和GB18030等向后兼容于GB2312的字符集时同理**
>
> **因为unicode字符集对应多种字符编码,所以当使用unicode来表达一套包含字符集和字符编码的方案时,是非常不合适的,使用UTF-8来表达稍微合适点,道理和上面使用GB2312同理**

这里简单介绍下UTF-8字符编码,不同的字符在UTF-8字符编码中所占的字节数不同,UTF-8字符编码会根据一串二进制数自动判断字符所占的字节数,规则如下:

> 1字节0xxxxxxx
>
> 2字节110xxxxx 10xxxxxx
>
> 3字节1110xxxx 10xxxxxx 10xxxxxx
>
> 4字节11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
>
> 5字节111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
>
> 6字节1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
>
> 英文字符在UTF-8中也是用不到128的数值表示的,只占一个字节,所以用UTF-8存英文字符串比较省空间

linux中如果安装时选择中文环境,则默认都是UTF-8字符编码

windows对字符编码的命名极其混乱,如果想让文本文件获得良好的通用性,**千万别用windows的记事本**!推荐使用notepad++.

windows记事本默认使用ASNI字符编码,他代表使用当前环境默认字符编码,就是打开时是什么格式保存时就是什么格式,如果是新建,就使用windows默认中文字符编码是GB18030.保存时选择Unicode代表使用带BOM的UTF-16保存.选择UTF-8代表使用带BOM的UTF-8保存.

再介绍下BOM(Byte order mark),BOM也是Unicode标准的一部分,加在UTF系列的字符编码文件头,BOM标识了此文件属于哪种Unicode编码(UTF-8，UTF-16BE，UTF-16LE,BE和LE代表此字符编码的一个字符在内存或磁盘上是大端(Big-endian)还是小端(Little-endian)格式存储),UTF文件前加BOM是微软的习惯,在微软在 UTF-8 中使用 BOM 是因为这样可以把 UTF-8 和 ASCII 等编码明确区分开，但这样的文件在 Windows 之外的操作系统里会带来问题,如果在linux下想删除UTF文件的BOM,用vim打开文件后执行:

```vim
:set nobomb
:w
```

[维基BOM详细介绍](https://en.wikipedia.org/wiki/Byte_order_mark)

### linux下转换字符编码工具

iconv

> -f	输入字符编码方案
>
> -t	输出字符编码方案

例子

```shell
#windows下使用git bash中git diff查看gbk编码方案的中文文件时,会出现乱码,使用iconv转换
git diff | iconv -f gbk -t utf-8
```

## 2.vim

### 1.vim编码相关参数

在Vim中，有四个与编码有关的选项，它们是：fileencodings、fileencoding、encoding和termencoding。在实际使用中，任何一个选项出现错误，都会导致出现乱码。因此，每一个Vim用户都应该明确这四个选项的含义。下面，我们详细介绍一下这四个选项的含义和作用。

* encoding

　　encoding是Vim内部使用的字符编码方式。当我们设置了encoding之后，Vim内部所有的buffer、寄存器、脚本中的字符串等，全都使用这个编码。Vim 在工作的时候，如果编码方式与它的内部编码不一致，它会先把编码转换成内部编码。如果工作用的编码中含有无法转换为内部编码的字符，在这些字符就会丢失。因此，在选择 Vim 的内部编码的时候，一定要使用一种表现能力足够强的编码，以免影响正常工作。

　　由于encoding选项涉及到Vim中所有字符的内部表示，因此只能在Vim启动的时候设置一次。在Vim工作过程中修改encoding会造成非常多的问题。用户手册上建议只在 .vimrc中改变它的值，事实上似乎也只有在 .vimrc中改变它的值才有意义。如果没有特别的理由，请始终将encoding设置为utf-8。为了避免在非UTF-8的系统如Windows下，菜单和系统提示出现乱码，可同时做这几项设置：

> set encoding=utf-8
> set langmenu=zh_CN.UTF-8
> language message zh_CN.UTF-8

* termencoding

　　termencoding是Vim用于屏幕显示的编码，在显示的时候，Vim会把内部编码转换为屏幕编码，再用于输出。内部编码中含有无法转换为屏幕编码的字符时，该字符会变成问号，但不会影响对它的编辑操作。如果termencoding没有设置，则直接使用encoding不进行转换。

　　举个例子，当你在Windows下通过telnet登录Linux工作站时，由于Windows的telnet是GBK编码的，而Linux下使用UTF-8编码，你在telnet下的Vim中就会乱码。此时有两种消除乱码的方式：一是把Vim的encoding改为gbk，另一种方法是保持encoding为utf-8，把termencoding改为gbk，让Vim在显示的时候转码。显然，使用前一种方法时，如果遇到编辑的文件中含有GBK无法表示的字符时，这些字符就会丢失。但如果使用后一种方法，虽然由于终端所限，这些字符无法显示，但在编辑过程中这些字符是不会丢失的。

　　对于图形界面下的GVim，它的显示不依赖TERM，因此termencoding对于它没有意义。在GTK2下的GVim 中，termencoding永远是utf-8，并且不能修改。而Windows下的GVim则忽略termencoding的存在。

* fileencoding

　　当Vim从磁盘上读取文件的时候，会对文件的编码进行探测。如果文件的编码方式和Vim的内部编码方式不同，Vim就会对编码进行转换。转换完毕后，Vim会将fileencoding选项设置为文件的编码。当Vim存盘的时候，如果encoding和fileencoding不一样，Vim就会进行编码转换。因此，通过打开文件后设置fileencoding，我们可以将文件由一种编码转换为另一种编码。但是，由前面的介绍可以看出，fileencoding是在打开文件的时候，由Vim进行探测后自动设置的。因此，如果出现乱码，我们无法通过在打开文件后重新设置fileencoding来纠正乱码。

　　简而言之，fileencoding是Vim中当前编辑的文件的字符编码方式，Vim保存文件时也会将文件保存为这种字符编码方式 （不管是否新文件都如此）。

* fileencodings

　　编码的自动识别是通过设置fileencodings实现的，注意是复数形式。fileencodings是一个用逗号分隔的列表，列表中的每一项是一种编码的名称。当我们打开文件的时候，VIM按顺序使用fileencodings中的编码进行尝试解码，如果成功的话，就使用该编码方式进行解码，并将fileencoding设置为这个值，如果失败的话，就继续试验下一个编码。

　　因此，我们在设置fileencodings的时候，一定要把要求严格的、当文件不是这个编码的时候更容易出现解码失败的编码方式放在前面，把宽松的编码方式放在后面。例如，latin1是一种非常宽松的编码方式，任何一种编码方式得到的文本，用latin1进行解码，都不会发生解码失败——当然，解码得到的结果自然也就是理所当然的“乱码”。因此，如果你把latin1放到了fileencodings的第一位的话，打开任何中文文件都是乱码也就是理所当然的了。

### 2.vim推荐配置

以下是网上推荐的一个fileencodings设置：

> set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,euc-jp,euc-kr,latin1

　　其中，ucs-bom是一种非常严格的编码，非该编码的文件几乎没有可能被误判为ucs-bom，因此放在第一位。

　　utf-8也相当严格，除了很短的文件外（例如许多人津津乐道的GBK编码的“联通”被误判为UTF-8编码的经典错误），现实生活中一般文件是几乎不可能被误判的，因此放在第二位。

　　接下来是cp936和gb18030，这两种编码相对宽松，如果放前面的话，会出现大量误判，所以就让它们靠后一些。cp936的编码空间比gb18030小，所以把cp936放在gb18030前面。

　　至于big5、euc-jp和euc-kr，它们的严格程度和cp936差不多，把它们放在后面，在编辑这些编码的文件的时候必然出现大量误判，但这是Vim内置编码探测机制没有办法解决的事。由于中国用户很少有机会编辑这些编码的文件，因此我们还是决定把cp936和gb18030放在前面以保证这些编码的识别。

　　最后就是latin1了。它是一种极其宽松的编码，以至于我们不得不把它放在最后一位。不过可惜的是，当你碰到一个真的latin1编码的文件时，绝大部分情况下，它没有机会fall-back到latin1，往往在前面的编码中就被误判了。不过，正如前面所说的，中国用户没有太多机会接触这样的文件。

　　如果编码被误判了，解码后的结果就无法被人类识别，于是我们就说，这个文件乱码了。此时，如果你知道这个文件的正确编码的话，可以在打开文件的时候使用 ++enc=encoding 的方式来打开文件，如：

```vim
:e ++enc=utf-8 myfile.txt
```

　　上面就是Linux解决Vim显示utf-8文档乱码的方法介绍了，出现该乱码问题后，可通过重新设置fileencodings来解决，希望对你有所帮助。

### 3.Vim的工作原理

好了，解释完了这一堆easy让新手犯糊涂的參数，我们来看看Vim的多字符编码方式支持是怎样工作的。

1. Vim启动，依据 .vimrc中设置的encoding的值来设置buffer、菜单文本、消息文的字符编码方式。
2. 读取须要编辑的文件，依据fileencodings中列出的字符编码方式逐一探測该文件编码方式。并设置fileencoding为探測到的，看起来是正确的字符编码方式。其实，Vim 的探測精确度并不高，尤其是在encoding没有设置为utf-8时。因此强烈建议将encoding设置为utf-8，假设你想Vim显示中文菜单和提示消息的话这样会带来还有一个小问题。

3. 对照fileencoding和encoding的值，若不同则调用iconv将文件内容转换为encoding所描写叙述的字符编码方式，而且把转换后的内容放到为此文件开辟的buffer里，然后根据termencoding指定的终端编码方式，转换成termencoding编码后，写入到终端,此时我们就能够開始编辑这个文件了。注意。完毕这一步动作须要调用外部iconv,你须要保证这个文件存在于$VIMRUNTIME或者其它列在PATH环境变量中的文件夹里。
4. 编辑完毕后保存文件时。再次对照fileencoding和encoding的值。若不同，再次调用iconv将即将保存的buffer中的文本转换为fileencoding所描写叙述的字符编码方式，并保存到指定的文件里。相同，这须要调用iconv

可以看出，VIM涉及到的3种字符编码之间的转换:

读:fileencoding—–> encoding

显:encoding ——> termencoding

写:encoding ——-> fileencoding

### 4.常见乱码情况分析

#### (1)读文件时，VIM探测fileencoding不准确

这很好理解，比如以GBK编码方式存储的文件，VIM把fileencoding探测成了ASCII，则肯定会出现问题。

【解决方法】一是靠VIM自身提高探测水平；二是设置合适的fileencodings变量，把最可能用到的编码方式放到最前面。如果VIM实在是探测不对，那么就只能通过 :set fileencoding=xxx 命令来手动探测了。

#### (2)fileencoding编码无法正确转换到encoding编码

例如，文件采用GBK编码，而ecoding使用ASCII，这样大量的汉字字符无法被转换，从而导致乱码。

【解决方法】把encoding设置成UTF-8，目前为止UTF-8能包含所有字符，所以其他的任何编码方式都可以无损的转换为UTF-8。

#### (3)encoding无法正确转换到termencoding

这个问题，与(2)类似.

【解决办法】把termencoding设置为和encoding相同。默认termencoding=”“的情况下，这两者就是相同的。

#### (4)termencoding与实际的终端字符编码不一致

例如本来字符终端的编码属性为GBK，而termencoding却为UTF-8，那么VIM就会错误的认为终端就是UTF-8编码的，导致向终端输出UTF-8编码的字节流，而终端却按照GBK来识别，当然就会识别成乱码.

【解决办法】把终端实际的编码方式和VIM的termencoding统一起来。

####　(5)终端显示能力欠缺

例如，传统的字符终端，本身不具备显示汉字的能力，虽然它可以识别出UTF-8编码的汉字，但是渲染引擎无法正确绘制，也就显示成了乱码。

【解决办法】尽量还是使用Putty等伪终端软件，避免直接使用字符终端设备；如果实在不能避免，就要避免使用ASCII字符集以外的字符，好好学习英文吧。

### 5.解决的方法演示样例

#### (1)方法一：设定.vimrc文件：

​        在/home/username/.vimrc或者/root/.vimrc下添加两句话：

> let &termencoding=&encoding
>
> set fileencodings=utf-8,gbk,ucs-bom,cp936

​        这样的办法能够实现编辑UTF-8文件

#### (2)方法而二：打开文件后，在vi编辑器中设定：

> :set encoding=utf-8 termencoding=gbk fileencoding=utf-8

#### (3)方法三：新建UTF-8文件，在vi编辑器设定：

> :set fenc=utf-8
>
> :set enc=GB2312

​        这样在编辑器里输入中文，保存的文件是UTF-8。

#### (4)方法四：一个推荐的～/.vimrc文件配置：

> set encoding=utf-8
>
> set fileencodings=ucs-bom,utf-8,cp936,gb18030,latin1
>
> set termencoding=gb18030
>
> set expandtab
>
> set ts=4
>
> set shiftwidth=4
>
> set nu
>
> syntax on
>
> if has('mouse')
>
> set mouse-=a
>
> endif



## 参考

[VIM显示utf-8文档乱码解决方法](https://www.cnblogs.com/brucemengbm/p/6709450.html)

[让VIM彻底告别乱码](https://blog.csdn.net/smstong/article/details/51279810)

[字符编码与字符集的区别](https://blog.csdn.net/qq_29028175/article/details/52959551)

[Windows 记事本的 ANSI、Unicode、UTF-8 这三种编码模式有什么区别？](https://www.zhihu.com/question/20650946)

[「带 BOM 的 UTF-8」和「无 BOM 的 UTF-8」有什么区别？网页代码一般使用哪个？](https://www.zhihu.com/question/20167122)