

# systemd介绍及使用

​systemd放弃了通过脚本来管理服务的使用习惯,虽然很多有经验的linux用户很不习惯,但systemd太强大了,systemd统一linux初始化系统是大势所趋





# 环境

* 系统:ubuntu 18.04(本文中/lib/systemd/system/目录在Centos7中为/usr/lib/systemd/system目录)
* 内核版本:4.15.0-24-generic
* 权限为root



## 介绍

systemd 是 Linux 下一个与 SysV 和 LSB 初始化脚本兼容的系统和服务管理器。systemd 使用 socket 和 D-Bus 来开启服务，提供基于守护进程的按需启动策略，保留了 Linux cgroups 的进程追踪功能，支持快照和系统状态恢复，维护挂载和自挂载点，实现了各服务间基于从属关系的一个更为精细的逻辑控制，拥有前卫的并行性能。systemd 无需经过任何修改便可以替代 sysvinit 。 



# 特点

1. 以`systemctl`命令管理所有unit功能
2. 兼容system V
3. 更快的启动速度(尽可能启动更少的进程,尽可能将更多进程并行启动),systemd抛弃system V以脚本顺序启动系统的方式,有非常强大的并发启动能力,所以开机很快(以字符模式启动时,可能会出现tty1登录终端,输入账号密码后无法登录的情况,因为systemd-logind.service 或 systemd-user-sessions.service还没启动)
4. 自带自动挂载功能(systemV依靠autofs服务实现自动挂载),兼容/etc/fstab的开机自动挂载方式
5. Systemd 采用 Linux 的 Cgroup 特性跟踪和管理进程的生命周期
6. 自带日志服务journald,自带提供定时任务功能的timer单元
7. 系统快照功能



# 无法取代systemV的部分

1. sysV的runlevel无法和systemd完全对应上
2. sysV的`/etc/init.d/daemon`脚本可以自定义参数,而`systemctl`命令不可以
3. 如果某个任务是管理员手动执行而不是使用`systemctl`启动的,或者kill杀掉的,systemd将无法在正确管理此服务(这里的手动执行可能是指直接执行sysV脚本的方式)
4. systemd 啟動過程中，無法與管理員透過 standard input 傳入訊息！因此，自行撰寫 systemd 的啟動設定時，務必要取消互動機制～(連透過啟動時傳進的標準輸入訊息也要避免！)(不太理解)



# 单元的概念

系统初始化所要做的所有内容都被抽象化为不同的unit

## unit类型

* service ：代表一个后台服务进程，比如 mysqld。这是最常用的一类。
* target ：此类配置单元代表着若干个其他类型配置单元的集合。它们本身实际上并不做什么，只是引用其他配置单元而已。这样便可以对配置单元做一个统一的控制。这样就可以实现大家都已经非常熟悉的运行级别概念。比如想让系统进入图形化模式，需要运行许多服务和配置命令，这些操作都由一个个的配置单元表示，将所有这些配置单元组合为一个目标(target)，就表示需要将这些配置单元全部执行一遍以便进入目标所代表的系统运行状态。 (例如：multi-user.target 相当于在传统使用 SysV 的系统中运行级别 5)
* device ：此类配置单元封装一个存在于 Linux 设备树中的设备。每一个使用 udev 规则标记的设备都将会在 systemd 中作为一个设备配置单元出现。
* socket ：此类配置单元封装系统和互联网中的一个 套接字 。当下，systemd 支持流式、数据报和连续包的 AF_INET、AF_INET6、AF_UNIX socket 。每一个套接字配置单元都有一个相应的服务配置单元 。相应的服务在第一个"连接"进入套接字时就会启动(例如：nscd.socket 在有新连接后便启动 nscd.service),有点类似与sysV上的xinetd这个super daemon.
* mount,automount ：此类配置单元封装系统结构层次中的一个自挂载点。每一个自挂载配置单元对应一个挂载配置单元 ，当该自动挂载点被访问时，systemd 执行挂载点中定义的挂载行为。
* swap: 和挂载配置单元类似，交换配置单元用来管理交换分区。用户可以用交换配置单元来定义系统中的交换分区，可以让这些交换分区在启动时被激活。
* timer：定时器配置单元用来定时触发用户定义的操作，这类配置单元取代了 atd、crond 等传统的定时服务。
* snapshot ：与 target 配置单元相似，快照是一组配置单元。它保存了系统当前的运行状态。
* .path:监测特定档案或目录的配置单元,某些服务需要监测某些特定的服务或文件来提供队列服务,比如最常见的打印服务,就是通过监测队列目录来启动打印功能.

## unit文件位置

* /lib/systemd/system

系统预设的unit位置,此目录下的文件尽量别修改,如果需要修改unit,redhat官方文件建议请到/etc/systemd/system目录下创建同名然后加上.d结尾的目录,在这个目录里面新建xxx.conf文件.如果需要在启动此unit之前启动其他unit,在/etc/systemd/system目录下创建同名然后加上.requires结尾的目录,在里面新建其他unit的软连接.如果需要启动完此unit后再启动的其他unit,在/etc/systemd/system目录下创建同名然后加上.wants结尾的目录,在里面新建其他unit的软连接.

* /etc/systemd/system

管理员安装的unit位置,此目录下的unit文件优先级更高,如果/lib/systemd/system和/etc/systemd/system目录下存在同名unit文件,以/etc/systemd/system目录下的unit文件为准.

上面说的在/lib/systemd/system下新建.d,.wants,.requires目录太麻烦,可以直接在/etc/systemd/system目录下新建同名文件覆盖/lib/systemd/system下unit的配置







# 启动模式

`systemctl [command] [unit.target]`

> ```
> command:
>     get-default ：取得目前的 target 
>     set-default ：設定後面接的 target 成為預設的操作模式
>     isolate     ：切換到後面接的模式
> ```

## 默认的启动模式文件:

`/etc/systemd/system/default.target`

是一个软连接,指向/lib/systemd/system目录下的target文件

可以通过下面命令查看与system V兼容的启动模式:

```shell
ll /lib/systemd/system/runlevel*.target 
```

也是软连接,,指向/lib/systemd/system目录下对应的target文件

## 切换启动模式

| SystemV    | systemd                             |
| ---------- | ----------------------------------- |
| init 0     | systemctl poweroff                  |
| init 1     | systemctl rescue                    |
| init [234] | systemctl isolate multi-user.target |
| init 5     | systemctl isolate graphical.target  |
| init 6     | systemctl reboot                    |

* 不同启动模式下的对应的启动服务目录:

```shell
#此目录下为系统预设的加载服务目录
ll /lib/systemd/system/*.target.wants

##此目录下为使用者设定的加载服务目录
ll /etc/systemd/system/*.target.wants
```

执行`systemctl enable foo.service`和`systemctl disable foo.service`命令时,就是在管理foo.service的target的wants目录下新建和删除名为foo.service的软链接

## Systemd 电源管理命令

| 命令                   | 操作                                 |
| ---------------------- | ------------------------------------ |
| systemctl reboot       | 重启机器                             |
| systemctl poweroff     | 关机                                 |
| systemctl suspend      | 待机                                 |
| systemctl hibernate    | 休眠                                 |
| systemctl hybrid-sleep | 混合休眠模式（同时休眠到硬盘并待机） |
| systemctl rescue       | 強制進入救援模式                     |
| systemctl emergency    | 強制進入緊急救援模式                 |

## graphical.target启动流程

1. local-fs.target + swap.target
2. sysinit.target
3. basic.target
4. multi-user.target
5. graphical.target





# Systemd unit的管理

## 查看unit的依赖关系

`systemctl list-dependencies [unit] [--reverse]`

> ```
> --reverse ：反向追蹤誰使用這個 unit 的意思！
> ```

## 查看unit的参数

`systemctl show unit`

## Systemd命令和sysvinit对照表

| Sysvinit 命令           | Systemd 命令                                                 | 备注                                                    |
| ----------------------- | ------------------------------------------------------------ | ------------------------------------------------------- |
| service foo start       | systemctl start foo.service                                  | 用来启动一个服务 (并不会重启现有的)                     |
| service foo stop        | systemctl stop foo.service                                   | 用来停止一个服务 (并不会重启现有的)。                   |
| service foo restart     | systemctl restart foo.service                                | 用来停止并启动一个服务。                                |
| service foo reload      | systemctl reload foo.service                                 | 当支持时，重新装载配置文件而不中断等待操作。            |
| service foo condrestart | systemctl condrestart foo.service                            | 如果服务正在运行那么重启它。                            |
| service foo status      | systemctl is-active foo.service                              | 查看服务是否运行,简单显示                               |
| service foo status      | systemctl status foo.service                                 | 查看服务是否运行,同时显示开机是否启动,进程号,日志等信息 |
| ls /etc/rc.d/init.d/    | systemctl list-unit-files --type=service                     | 用来列出可以启动或停止的服务列表。                      |
| chkconfig –list         | systemctl list-unit-files --type=service                     | 输出在各个运行级别下服务的启用和禁用情况                |
| chkconfig foo on        | systemctl enable foo.service                                 | 在下次启动时或满足其他触发条件时设置服务为启用          |
| chkconfig foo off       | systemctl disable foo.service                                | 在下次启动时或满足其他触发条件时设置服务为禁用          |
| chkconfig foo           | systemctl is-enabled foo.service                             | 用来检查一个服务在当前环境下被配置为开机启用还是禁用。  |
| chkconfig foo –list     | ls /etc/systemd/system/*.wants/foo.service                   | 用来列出该服务在哪些运行级别下启用和禁用。              |
| chkconfig foo –add      | systemctl daemon-reload                                      | 当您创建新服务文件或者变更设置时使用。                  |
| telinit 3               | systemctl isolate multi-user.target (OR systemctl isolate runlevel3.target OR telinit 3) | 改变至多用户运行级别。                                  |

## systemctl查看服务

`systemctl [command] [--type=TYPE] [--all]`

> ```
> command:
>     list-units      ：依據 unit 列出目前有啟動的 unit。若加上 --all 才會列出沒啟動的。
>     list-unit-files ：显示/lib/systemd/system/目录下所有unit的开机状态
>     list-sockets	: 查看和socket相关的服务和文件信息
> --type=TYPE：就是之前提到的 unit type，主要有 service, socket, target 等
> ```

执行`systemctl`命令和`systemctl list-units`命令相同

### 列出程序开机占用时间

执行

```shell
systemd-analyze blame
```

### 服务的状态

执行

```shell
systemctl status foo.service
```

#### 第三行Active

| ACtive           | 解释                                                         |
| ---------------- | ------------------------------------------------------------ |
| active (running) | 正有一隻或多隻程序正在系統中執行的意思，舉例來說，正在執行中的 vsftpd 就是這種模式。 |
| active (exited)  | 僅執行一次就正常結束的服務，目前並沒有任何程序在系統中執行。 舉例來說，開機或者是掛載時才會進行一次的 quotaon 功能，就是這種模式！ quotaon 不須一直執行～只須執行一次之後，就交給檔案系統去自行處理囉！通常用 bash shell 寫的小型服務，大多是屬於這種類型 (無須常駐記憶體)。 |
| active (waiting) | 正在執行當中，不過還再等待其他的事件才能繼續處理。舉例來說，列印的佇列相關服務就是這種狀態！ 雖然正在啟動中，不過，也需要真的有佇列進來 (列印工作) 這樣他才會繼續喚醒印表機服務來進行下一步列印的功能。 |
| inactive         | 這個服務目前沒有運作的意思。                                 |

#### 第二行Loaded

| Loaded   | 解释                                                         |
| -------- | ------------------------------------------------------------ |
| enabled  | 這個 daemon 將在開機時被執行                                 |
| disabled | 這個 daemon 在開機時不會被執行                               |
| static   | 這個 daemon 不可以自己啟動 (enable 不可)，不過可能會被其他的 enabled 的服務來喚醒 (相依屬性的服務) |
| mask     | 這個 daemon 無論如何都無法被啟動！因為已經被強制註銷 (非刪除)。可透過 systemctl unmask 方式改回原本狀態 |

## 强制禁止服务

` systemctl mask foo.service`

原理就是在/etc/systemd/system目录下新建一个指向/dev/null名为foo.service的软链接文件

取消强制禁止:

`systemctl unmask foo.service`

## 部分和开机相关的service介绍

### rc-local.service

systemd系统中可以写一个自己的unit放到`/etc/systemd/system`或`/lib/systemd/system`目录下,然后执行`systemd enable xxx`,这条命令的原理就是在当前启动模式下的target.wants目录中加入target.wants目录的上级目录下的unit文件的软连接

sysV初始化系统中,可以把开机启动程序的命令写在`/etc/rc.local`或`/etc/rc.d/rc.local文件`中,ubuntu18.04默认是没有这个文件的,因为systemd兼容sysV,可以自己手动创建`/etc/rc.local文件`,systemd的`rc-local.service`在`multi-user.target`中默认启动,它会动判断是否存在`/etc/rc.locl`文件和此文件是否有可执行权限,如果上述条件都为真,就执行此`rc.local`脚本

### systemd-modules-load.service

`systemd-modules-load.service`在`sysinit.target`中默认启动,

启动时会加载使用者设定在

`/etc/modules-load.d/*.conf`(这里的文件中只能写内核模块名)

和`/etc/modprobe.d/*.conf`(这里的文件中可以写内核模块名和一些参数)文件中的内核模块,修改完上述文件后,如果不想重启测试,可以执行

```shell
systemctl restart systemd-modules-load.service
```

来加载刚配置的内核模块



# unit文件解释

unit分三部分:

1. Unit
2. Service,Socket,Timer,Mount,Path等,不同的unit包含不同的部分
3. Install

## 规则

* 不同的参数可以重复,不过后面的会覆盖前面的,比如设置了多个After,最后一个是"After=",那么After就是空,因为最后的After覆盖了前面设置的After
* 可以用 1, yes, true, on代表真,0, no, false, off代表假
* 开头为#或;的那一行,都代表注释
* 在unit文件中,.include代表引用,可以看下面例子:

```shell
cp /lib/systemd/system/lighttpd.service /etc/systemd/system/lighttpd.service
cat /etc/systemd/system/lighttpd.service
.include /lib/systemd/system/lighttpd.service 
[Service]
#因为/etc/systemd/system下面的优先级更高,而且参数可以覆盖,所以lightttp的Nice的参数就是下面的-5
Nice=-5
```

* unit文件有自己的语法,支持变量,参数功能

## Unit部分

| 参数          | 描述                                                         |
| ------------- | ------------------------------------------------------------ |
| Description   | 描述,使用`systemctl list-units`和`systemctl status`时能看到  |
| Documentation | 参考手册                                                     |
| Requires      | 依赖后面的unit,必须等后面的unit启动后才能启动此unit,后面的unit停止的时候,此unit也必须停止 |
| Wants         | 和Requires相反,进行完此unit后,还需要启动后面的unit,仅是建议,无强制要求的意思 |
| After         | 后面的unit启动之后才会启动此unit,仅代表顺序,如果after之后的unit没有启动也可以启动此unit,和强制要求的requires不同 |
| Before        | 与after相反,表示后面的unit启动前最好启动此unit,无强制要求的意思 |
| Conflicts     | 和后面的unit有冲突,如果后面的unit启动了,此unit就不能启动     |

## Install部分

| 参数     | 描述                                                         |
| -------- | ------------------------------------------------------------ |
| WantedBy | 這個設定後面接的大部分是 *.target unit ！意思是，這個 unit 本身是附掛在哪一個 target unit 底下的！一般來說，大多的服務性質的 unit 都是附掛在 multi-user.target 底下！ |
| Also     | 當目前這個 unit 本身被 enable 時，Also 後面接的 unit 也請 enable 的意思！也就是具有相依性的服務可以寫在這裡呢！ |
| Alias    | 進行一個連結的別名的意思！當 systemctl enable 相關的服務時，則此服務會進行連結檔的建立！以 multi-user.target 為例，這個傢伙是用來作為預設操作環境 default.target 的規劃， 因此當你設定用成 default.target 時，這個 /etc/systemd/system/default.target 就會連結到 /usr/lib/systemd/system/multi-user.target 囉！ |

## service部分

| 参数            | 描述                                                         |
| --------------- | ------------------------------------------------------------ |
| Type            | 說明這個 daemon 啟動的方式，會影響到 ExecStart 喔！一般來說，有底下幾種類型:<br />simple：預設值，這個 daemon 主要由 ExecStart 接的指令串來啟動，啟動後常駐於記憶體中。<br />forking：由 ExecStart 啟動的程序透過 spawns 延伸出其他子程序來作為此 daemon 的主要服務。原生的父程序在啟動結束後就會終止運作。 傳統的 unit 服務大多屬於這種項目，例如 httpd 這個 WWW 服務，當 httpd 的程序因為運作過久因此即將終結了，則 systemd 會再重新生出另一個子程序持續運作後， 再將父程序刪除。據說這樣的效能比較好！！<br />oneshot：與 simple 類似，不過這個程序在工作完畢後就結束了，不會常駐在記憶體中。<br />dbus：與 simple 類似，但這個 daemon 必須要在取得一個 D-Bus 的名稱後，才會繼續運作！因此設定這個項目時，通常也要設定 BusName= 才行！<br />idle：與 simple 類似，意思是，要執行這個 daemon 必須要所有的工作都順利執行完畢後才會執行。這類的 daemon 通常是開機到最後才執行即可的服務！<br />比較重要的項目大概是 simple, forking 與 oneshot 了！畢竟很多服務需要子程序 (forking)，而有更多的動作只需要在開機的時候執行一次(oneshot)，例如檔案系統的檢查與掛載啊等等的。 |
| EnvironmentFile | 可以指定啟動腳本的環境設定檔！例如 sshd.service 的設定檔寫入到 /etc/sysconfig/sshd 當中！你也可以使用 Environment= 後面接多個不同的 Shell 變數來給予設定！ |
| ExecStart       | 就是實際執行此 daemon 的指令或腳本程式。你也可以使用 ExecStartPre (之前) 以及 ExecStartPost (之後) 兩個設定項目來在實際啟動服務前，進行額外的指令行為。 但是你得要特別注意的是，指令串僅接受『指令 參數 參數...』的格式，不能接受 <, >, >>, \|, & 等特殊字符，很多的 bash 語法也不支援喔！ 所以，要使用這些特殊的字符時，最好直接寫入到指令腳本裡面去！不過，上述的語法也不是完全不能用，亦即，若要支援比較完整的 bash 語法，那你得要使用 Type=oneshot 才行喔！ 其他的 Type 才不能支援這些字符。 |
| ExecStop        | 與 systemctl stop 的執行有關，關閉此服務時所進行的指令。     |
| ExecReload      | 與 systemctl reload 有關的指令行為                           |
| Restart         | 當設定 Restart=1 時，則當此 daemon 服務終止後，會再次的啟動此服務。舉例來說，如果你在 tty2 使用文字界面登入，操作完畢後登出，基本上，這個時候 tty2 就已經結束服務了。 但是你會看到螢幕又立刻產生一個新的 tty2 的登入畫面等待你的登入！那就是 Restart 的功能！除非使用 systemctl 強制將此服務關閉，否則這個服務會源源不絕的一直重複產生！ |
| RemainAfterExit | 當設定為 RemainAfterExit=1 時，則當這個 daemon 所屬的所有程序都終止之後，此服務會再嘗試啟動。這對於 Type=oneshot 的服務很有幫助！ |
| TimeoutSec      | 若這個服務在啟動或者是關閉時，因為某些緣故導致無法順利『正常啟動或正常結束』的情況下，則我們要等多久才進入『強制結束』的狀態！ |
| KillMode        | 可以是 process, control-group, none 的其中一種，如果是 process 則 daemon 終止時，只會終止主要的程序 (ExecStart 接的後面那串指令)，如果是 control-group 時， 則由此 daemon 所產生的其他 control-group 的程序，也都會被關閉。如果是 none 的話，則沒有程序會被關閉喔！ |
| RestartSec      | 與 Restart 有點相關性，如果這個服務被關閉，然後需要重新啟動時，大概要 sleep 多少時間再重新啟動的意思。預設是 100ms (毫秒)。 |

## Timer部分

| 参数              | 描述                                                         |
| ----------------- | ------------------------------------------------------------ |
| OnActiveSec       | 當 timers.target 啟動多久之後才執行這隻 unit                 |
| OnBootSec         | 當開機完成後多久之後才執行                                   |
| OnStartupSec      | 當 systemd 第一次啟動之後過多久才執行                        |
| OnUnitActiveSec   | 這個 timer 設定檔所管理的那個 unit 服務在最後一次啟動後，隔多久後再執行一次的意思 |
| OnUnitInactiveSec | 這個 timer 設定檔所管理的那個 unit 服務在最後一次停止後，隔多久再執行一次的意思。 |
| OnCalendar        | 使用實際時間 (非循環時間) 的方式來啟動服務的意思！至於時間的格式後續再來談。 |
| Unit              | 一般來說不太需要設定，因此如同上面剛剛提到的，基本上我們設定都是 sname.service + sname.timer，那如果你的 sname 並不相同時，那在 .timer 的檔案中， 就得要指定是哪一個 service unit 囉！ |
| Persistent        | 當使用 OnCalendar 的設定時，指定該功能要不要持續進行的意思。通常是設定為 yes ，比較能夠滿足類似 anacron 的功能喔！ |

foo.timer对应foo.service服务,如果timer文件对应的service文件不叫foo.service,才需要在timer文件里指定Unit参数

* 一个循环时间运行的例子:

>現在假設這樣：
>
>- 開機後 2 小時開始執行一次這個 backup.service
>- 自從第一次執行後，未來我每兩天要執行一次 backup.service
>
>好了，那麼應該如何處理這個腳本呢？可以這樣做喔！
>
>```shell
>[root@study ~]# vim /etc/systemd/system/backup.timer
>[Unit]
>Description=backup my server timer
>
>[Timer]
>OnBootSec=2hrs
>OnUnitActiveSec=2days
>
>[Install]
>WantedBy=multi-user.target
># 只要這樣設定就夠了！儲存離開吧！
>
>[root@study ~]# systemctl daemon-reload
>[root@study ~]# systemctl enable backup.timer
>[root@study ~]# systemctl restart backup.timer
>[root@study ~]# systemctl list-unit-files | grep backup
>backup.service          disabled   # 這個不需要啟動！只要 enable backup.timer 即可！
>backup.timer            enabled
>
>[root@study ~]# systemctl show timers.target
>ConditionTimestamp=Thu 2015-08-13 14:31:11 CST      # timer 這個 unit 啟動的時間！
>
>[root@study ~]# systemctl show backup.service
>ExecMainExitTimestamp=Thu 2015-08-13 14:50:19 CST   # backup.service 上次執行的時間
>
>[root@study ~]# systemctl show backup.timer
>NextElapseUSecMonotonic=2d 19min 11.540653s         # 下一次執行距離 timers.target 的時間
>```
>
>
>
>如上表所示，我上次執行 backup.service 的時間是在 2015-08-13 14:50 ，由於設定兩個小時執行一次，因此下次應該是 2015-08-15 14:50 執行才對！ 由於 timer 是由 timers.target 這個 unit 所管理的，而這個 timers.target 的啟動時間是在 2015-08-13 14:31 ， 要注意，最終 backup.timer 所紀錄的下次執行時間，其實是與 timers.target 所紀錄的時間差！因此是『 2015-08-15 14:50 - 2015-08-13 14:31 』才對！ 所以時間差就是 2d 19min 囉！

* 一个固定日期运行例子:

> 上面的案例是固定週期運作一次，那如果我希望不管上面如何運作了，我都希望星期天凌晨 2 點運作這個備份程式一遍呢？請注意，因為已經存在 backup.timer 了！ 所以，這裡我用 backup2.timer 來做區隔喔！
>
> ```shell
> [root@study ~]# vim /etc/systemd/system/backup2.timer
> [Unit]
> Description=backup my server timer2
> 
> [Timer]
> OnCalendar=Sun *-*-* 02:00:00
> Persistent=true
> Unit=backup.service
> 
> [Install]
> WantedBy=multi-user.target
> 
> [root@study ~]# systemctl daemon-reload
> [root@study ~]# systemctl enable backup2.timer
> [root@study ~]# systemctl start backup2.timer
> [root@study ~]# systemctl show backup2.timer
> NextElapseUSecRealtime=45y 7month 1w 6d 10h 30min
> ```
>
> 與循環時間運作差異比較大的地方，在於這個 OnCalendar 的方法對照的時間並不是 times.target 的啟動時間，而是 Unix 標準時間！ 亦即是 1970-01-01 00:00:00 去比較的！因此，當你看到最後出現的 NextElapseUSecRealtime 時，哇！下一次執行還要 45 年 + 7 個月 + 1 周 + 6 天 + 10 小時過 30 分～剛看到的時候，鳥哥確實因此揉了揉眼睛～確定沒有看錯...這才了解原來比對的是『日曆時間』而不是某個 unit 的啟動時間啊！呵呵！
>
> 透過這樣的方式，你就可以使用 systemd 的 timer 來製作屬於你的時程規劃服務囉！

## Mount部分

| 参数          | 描述                                                         |
| ------------- | ------------------------------------------------------------ |
| What          | 挂什么,待挂载的块设备的绝对路径                              |
| Where         | 挂哪儿,挂载点的绝对路径,这个路径必须和unit文件名相同或类似,看下方的说明. |
| Type          | 挂载的文件类型                                               |
| DirectoryMode | 挂载点的访问模式,默认为0755                                  |

- systemd 的 unit 文件名要与 Where 中的挂载路径一致，对于 `aliyun-nas.mount` ，Where 中要写 `Where=/aliyun/nas`
- 如果连字符作为目录名的一部分，需要进行编码，对于 `aliyun-nas.mount` ，unit 文件名要用 `aliyun\x2dnas.mount` ，Where 中要写 `Where="/aliyun-nas"`（注：一定要加双引号）
- 一个mount单元例子:

> ```shell
> cat /etc/systemd/system/aliyun-nas.mount
> #this is mount example
> 
> [Unit]
> 
> Description=mnt mount(/mnt)
> 
> [Mount]
> 
> What=/dev/sda4
> 
> Where=/aliyun/nas
> 
> Type=vfat
> 
> [Install]
> 
> WantedBy=multi-user.target
> ```

## Automount部分

| 参数          | 描述                                                         |
| ------------- | ------------------------------------------------------------ |
| Where         | 挂载点的绝对路径,这个路径必须和unit文件名相同或类似,看mount部分下方的说明.如果这个挂载点不存在,而automount unit已经启动,这个目录会自动创建 |
| DirectoryMode | 挂载点的访问模式,默认为0755                                  |

automount文件需要和它同名的mount文件存在才能工作.

automount启动之后,执行`mount`,可看到一个systemd-1文件挂载在automount单元中where指定目录的结果,当进入此目录后,再执行`mount`,可看到和automount单元同名的mount单元中指定的文件系统已被挂载































# 参考资料

[浅析 Linux 初始化 init 系统，第 3 部分Systemd](https://www.ibm.com/developerworks/cn/linux/1407_liuming_init3/index.html?ca=drs-)

[鳥哥的 Linux 私房菜第4版第十九章、開機流程、模組管理與 Loader](http://linux.vbird.org/linux_basic/0510osloader.php#process_2)

[linux systemd 的"Where= setting doesn't match unit name. Refusing."问题](https://q.cnblogs.com/q/105028/)

man 5 systemd.unit

