# linux下proc目录下部分文件含义



| 文件名            | 文件内容                                                     |
| ----------------- | ------------------------------------------------------------ |
| /proc/cmdline     | 加载kernel
时所下达的相关参数！查阅此文件，可了解系统是如何启动的！ |
| /proc/cpuinfo     | 本机的CPU 的相关资讯，包含时脉、类型与运算功能等             |
| /proc/devices     | 这个文件记录了系统各个主要装置的主要装置代号，与mknod有关呢！ |
| /proc/filesystems | 目前系统已经加载的文件系统罗！                               |
| /proc/interrupts  | 目前系统上面的IRQ 分配状态。                                 |
| /proc/ioports     | 目前系统上面各个装置所配置的I/O 位址。                       |
| /proc/kcore       | 这个就是内存的大小啦！好大对吧！但是不要读他啦！             |
| /proc/loadavg     | 还记得top以及uptime吧？没错！上头的三个平均数值就是记录在此！ |
| /proc/meminfo     | 使用free列出的内存资讯，嘿嘿！在这里也能够查阅到！           |
| /proc/modules     | 目前我们的Linux 已经加载的模块列表，也可以想成是驱动程序啦！ |
| /proc/mounts      | 系统已经挂载的数据，就是用mount 这个命令呼叫出来的数据啦！   |
| /proc/swaps       | 到底系统挂加载的内存在哪里？呵呵！使用掉的partition 就记录在此啦！ |
| /proc/partitions  | 使用fdisk -l 会出现目前所有的partition 吧？在这个文件当中也有纪录喔！ |
| /proc/pci         | 在PCI 汇流排上面，每个装置的详细情况！可用
lspci 来查阅！     |
| /proc/uptime      | 就是用uptime 的时候，会出现的资讯啦！                        |
| /proc/version     | 核心的版本，就是用uname -a 显示的内容啦！                    |
| /proc/bus/*       | 一些汇流排的装置，还有U盘的装置也记录在此喔！                |

