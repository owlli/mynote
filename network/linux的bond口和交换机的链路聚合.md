# linux的bond口和交换机的链路聚合



通过网卡绑定（bond）技术，可以实现负载均衡、高可用目的。



## bond分类

### bond0

balance-rr，Round-robin policy，轮询策略，把数据包顺序地在每一块网卡上发送。

mac地址：bond网卡和所有组bond的物理网卡mac地址相同，这个mac地址来源于bond算法选择其中一块物理网卡的。



### bond1

active-backup，Active-backup policy，主备策略，只有一块主网卡被up，主网卡down掉后，新生成一个主网卡。

mac地址：bond网卡和所有组bond的物理网卡mac地址相同，这个mac地址来源于bond创建时第一块活动网卡的。



### bond2

XOR policy，默认选择网卡序号算法：（源mac地址 xor 目的mac地址）%网卡数量。

mac地址：bond网卡和所有组bond的物理网卡mac地址相同，这个mac地址来源于bond算法选择其中一块物理网卡的。



### bond3

broadcast



### bond4

802.3ad，动态链接聚合策略。



### bond5

balance-tlb



### bond6

balance-alb



## bond口和交换机链路聚合口匹配策略

| bond模式 | 交换机配置               |
| -------- | ------------------------ |
| 0        | 静态链路聚合             |
| 1        | access端口，无需特别配置 |
| 2        | 静态链路聚合             |
| 3        | 静态链路聚合             |
| 4        | 动态链路聚合             |
| 5        | access端口，无需特别配置 |
| 6        | access端口，无需特别配置 |

bond0和bond2可以兼容，原理应该是组静态链路聚合的多块网卡mac地址相同，无论包从哪个物理网卡收到，相当于从一块网卡上收到。比如a、b网卡组bond0，c、d网卡组bond2，a与c用网线相连，b与d用网线相连，bond0发出的包会依次发到c和d上，但bond2机器不会觉得收包有问题，因为c和d对于bond2机器来说，是同一块网卡。





































