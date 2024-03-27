## iptables的使用

### 介绍

linux的netfilter内核模块可以对网络数据包进行处理，iptables是对这个内核模块进行配置的用户态工具。

linux收到数据包后，根据数据包的目的地址和路由规则，数据包会走不同的链，每个链包含4张表，每个表可以挂在不同的链上，iptables可以对任意一张表进行策略配置。

![4表5链](./images/4表5链.jpg)

iptables的四个表`iptable_filter`，`iptable_mangle`，`iptable_nat`，`iptable_raw`，默认表是`filter`（没有指定表的时候就是filter表）

- `filter 表`：用来对数据包进行过滤，具体的规则要求决定如何处理一个数据包。
  对应的内核模块为：`iptable_filter`，其表内包括三个链：`input`、`forward`、`output`;
- `nat 表`：nat 全称：network address translation 网络地址转换，主要用来修改数据包的 IP 地址、端口号信息。
  对应的内核模块为：`iptable_nat`，其表内包括三个链：`prerouting`、`postrouting`、`output`;
- `mangle 表`：主要用来修改数据包的服务类型，生存周期，为数据包设置标记，实现流量整形、策略路由等。
  对应的内核模块为：`iptable_mangle`，其表内包括五个链：`prerouting`、`postrouting`、`input`、`output`、`forward`;
- `raw 表`：主要用来决定是否对数据包进行状态跟踪。
  对应的内核模块为：`iptable_raw`，其表内包括两个链：`output`、`prerouting`;

> raw表只使用在`PREROUTING`链和`OUTPUT`链上,因为优先级最高，从而可以对收到的数据包在系统进行ip_conntrack（连接跟踪）前进行处理。一但用户使用了raw表,在某个链上，raw表处理完后，将跳过NAT表和ip_conntrack处理，即不再做地址转换和数据包的链接跟踪处理了。RAW表可以应用在那些不需要做nat的情况下，以提高性能。

iptables的五个链`PREROUTING`，`INPUT`，`FORWARD`，`OUTPUT`，`POSTROUTING`。

- `input 链`：当收到访问防火墙本机地址的数据包时，将应用此链中的规则；
- `output 链`：当防火墙本机向外发送数据包时，将应用此链中的规则；
- `forward 链`：当收到需要通过防火中转发给其他地址的数据包时，将应用此链中的规则，注意如果需要实现forward转发需要开启Linux内核中的ip_forward功能；
- `prerouting 链`：在对数据包做路由选择之前，将应用此链中的规则；
- `postrouting 链`：在对数据包做路由选择之后，将应用此链中的规则；

### 常用选项参数

```shell
iptables [ -t 表名] 命令选项 [链名] [条件匹配] [-j 处理动作或跳转]
```

-t 接表名，如果不加-t 默认就是 –t filter

命令选项

> -L	列出指定链中所有的规则，未指定链名，则列出表中的所有链
>
> -v	显示详细信息，包括每条规则的匹配句数量和匹配字节数
>
> -x	在v的基础上，禁止自动换算单位（K,M）
>
> -n	只显示ip地址和端口号，不显示域名和服务名称
>
> -I	在指定链中插入一条新规则，未指定序号时默认作为第一条规则
>
> -A	在指定链的末尾添加一条新的规则
>
> -D	删除指定链中的某一条规则，按规则序号或内容确定要删除的规则
>
> -R	修改、替换指定链中的某一条规则，可指定规则序号或具体内容
>
> -F	清空指定链中所有的规则，未指定链名，则清空表中的所有链
>
> -P	设置指定链指定表的默认策略，注意默认策略拒绝动作只能是DROP，而不能是REJECT
>
> -X	删除指定表中用户自定义的规则链
>
> --line-numbers	查看规则列表时，同时显示规则在链中的顺序号

条件匹配

> -i	入站请求接口
>
> -o	出站请求接口
>
> -s	入站源地址
>
> -d	目标地址
>
> -p	指定规则协议，如tcp, udp,icmp等，可以使用all来指定所有协议
>
> --dport	目的端口，数据包的目的（dport）地址是80，就是要访问我本地的80端口
>
> --sport	来源端口 数据包的来源端口是（sport）80，就是对方的数据包是80端口发送过来的。

可以对条件取反，在选项前加`!`即可

处理动作

> ACCEPT	允许数据包通过。
>
> DROP	直接丢弃数据包，不给任何回应信息，这时候客户端会感觉自己的请求泥牛入海了，过了超时时间才会有反应。
>
> REJECT	拒绝数据包通过，必要时会给数据发送端一个响应的信息，客户端刚请求就会收到拒绝的信息。--reject-with：可以设置提示信息，拒绝时的提示信息。当不设置任何值时，默认值为icmp-port-unreachable。
>
> SNAT	源地址转换，解决内网用户用同一个公网地址上网的问题。
>
> MASQUERADE	是SNAT的一种特殊形式，适用于动态的、临时会变的ip上。
>
> DNAT	目标地址转换。
>
> REDIRECT	在本机做端口映射。
>
> LOG	在/var/log/messages文件中记录日志信息，然后将数据包传递给下一条规则，也就是说除了记录以外不对数据包做任何其他操作，仍然让下一条规则去匹配
>
> 一般不使用REJECT(拒绝)行为.REJECT会带来安全隐患

### 链中的匹配策略

1. 自上而下，按顺序匹配，需要注意的是，有三个动作会短路，提前结束这个链上的这个表的匹配，这三个动作分别是: ACCEPT 、REJECT、DROP；
2. 若在该链的表上找不到匹配的规则，则按该链的表上的默认策略处理。

![iptables选项参数](./images/iptables选项参数.png)

### 匹配扩展

iptables匹配时，还可以使用很多扩展模块来增强匹配功能，详情见man手册

```shell
man iptables-extensions
```

### 配置的保存

使用iptables命令配置的规则，只能临时生效，重启了iptables服务，就会失效。如果想保存当前的iptables规则，在安装了iptables-services rpm包的centos7系统下，可以执行命令：

service iptables save

将当前iptables的规则保存到/etc/sysconfig/iptables，每次iptables服务重启，都会加载这个配置文件中的规则。

也可以执行：

iptables-save > xxx.txt

iptables-restore < xxx.txt

分别将iptables保存和恢复。

### 使用例子

#### SNAT和DNAT应用

- SNAT（Source Network Address Translation）
  源地址转换，根据指定条件修改数据包的源IP地址。
- DNAT（Destination Network Address Translation）
  目的地址转换，根据指定条件修改数据包的目的IP地址。

网关和防火墙就是靠这种机制实现的。

下面是一个配置例子：

![snat实验](./images/snat实验.png)

![dnat实验](./images/dnat实验.png)

上图中防火墙服务器和server router是同一台机器，需要在这台机器上进行如下操作：

```shell
# 开启路由功能
echo 1  > /proc/sys/net/ipv4/ip_forward

# 配置SNAT策略，实现snat功能，将所有192.168.88.0这个网段的ip的源ip改为192.168.0.107
iptables -t nat -A POSTROUTING -s 192.168.88.0/24 -o ens33 -j SNAT --to-source 192.168.0.107
# 这里只需要对出去的包做源地址的映射，从外网发回来的包，不需要做任何转换，因为发出去时防火墙会记录这个转换，防火墙内部会维护一个连接跟踪表（connection tracking table），返回的数据包的目的ip会自动被转换成内网的ip

# 配置DNAT策略，发布内网的web服务器192.168.88.2
iptables -t nat -A PREROUTING -d 192.168.0.107 -p tcp --dport 80 -i ens33 -j DNAT --to-destination 192.168.88.2

# 配置DNAT的策略，发布内网的ssh堡垒机192.168.88.1，访问防火墙服务器的2021端口发布到内网的192.168.88.1的22号端口上
iptables -t nat -A PREROUTING -d 192.168.0.107 -p tcp --dport 2021 -i ens33 -j DNAT --to-destination 192.168.88.1:22

```

#### SNAT和MASQUERADE的区别

MASQUERADE，地址伪装，算是snat中的一种特例，可以实现自动化的snat。

snat，出口的ip地址范围可以是一个，也可以是多个，比如：

```shell
# 如下命令表示把所有10.8.0.0网段的数据包SNAT成192.168.5.3的ip然后发出去
iptables-t nat -A POSTROUTING -s 10.8.0.0/255.255.255.0 -o eth0 -j SNAT --to-source192.168.5.3

# 如下命令表示把所有10.8.0.0网段的数据包SNAT成192.168.5.3/192.168.5.4/192.168.5.5等几个ip然后发出去
iptables-t nat -A POSTROUTING -s 10.8.0.0/255.255.255.0 -o eth0 -j SNAT --to-source192.168.5.3-192.168.5.5
```

这就是SNAT的使用方法，即可以NAT成一个地址，也可以NAT成多个地址，但是，对于SNAT，不管是几个地址，必须明确的指定要SNAT的ip，假如当前系统用的是ADSL动态拨号方式，那么每次拨号，出口ip192.168.5.3都会改变，而且改变的幅度很大，不一定是192.168.5.3到192.168.5.5范围内的地址，这个时候如果按照现在的方式来配置iptables就会出现问题了，因为每次拨号后，服务器地址都会变化，而iptables规则内的ip是不会随着自动变化的，每次地址变化后都必须手工修改一次iptables，把规则里边的固定ip改成新的ip，这样是非常不好用的。

MASQUERADE就是针对这种场景而设计的，他的作用是，从服务器的网卡上，自动获取当前ip地址来做NAT。

比如下边的命令：

```shell
iptables-t nat -A POSTROUTING -s 10.8.0.0/255.255.255.0 -o eth0 -j MASQUERADE
```

如此配置的话，不用指定SNAT的目标ip了，不管现在eth0的出口获得了怎样的动态ip，MASQUERADE会自动读取eth0现在的ip地址然后做SNAT出去，这样就实现了很好的动态SNAT地址转换。





## 策略路由

策略路由依靠iptables对报文打标和ip rule对报文分配路由表实现，ipset可以方便iptables对多个ip进行管理

### ip的route和rule

1、ip的route和rule区别？

ip route是用来管理路由表的，ip rule是管理策略的，规则指向路由表，多个规则可以引用一个路由表，而且某些路由表可以没有策略指向它。如果系统管理员删除了指向某个路由表的所有规则，这个表就没有用了，但是仍然存在，直到里面的所有路由都被删除，它才会消失。

2、我们已经有了route命令，来设置路由信息，为什么还需要ip的route和rule

route命令一般是传统的路由，当来了一个数据包，传统的路由是根据目标地址，决定走哪一条路，这种方式不够灵活

还会有这种需求：根据报文大小，应用或IP源地址等属性来选择转发路径

这就是基于策略的路由，也可以叫做，基于策略的转发

比方说：http的上网数据从eth0进行转发，FTP的数据通过eth1进行转发

3、所以需要ip的route和rule，怎么做？

a、使用ip route在路由表中，增加一个路由（比如，创建一个路由表100，增加一个路由）

b、使用ip rule指定满足某个特征的数据包，使用路由表100

4、内核配置的缺省路由表

可以通过/etc/iproute2/rt_tables文件查看table id和table name的映射关系。如果管理员新增了一张路由表，需要在/etc/iproute2/rt_tables文件中为新路由表添加table id和table name的映射

* 0号表

table Id为0的系统保留表

* 253【default表】

default table 没特别指定的默认路由都放在该表，default路由表，该路由表是一个空的路由表，正常情况下保持该路由表为空即可

* 254【main表】

main table 没指明路由表的所有路由放在该表，main路由表就是使用 route -n 命令所展现的路由表

如果添加路由时没有指定该路由所属的路由表，则这条路由会被添加到main路由表中。配置好网卡的网络设置系统就会自动生成main路由表

* 255【local表】

local table 保存本地接口地址，广播地址、NAT地址 由系统维护，用户不得更改 

5、内核配置的缺省的路由策略规则

* 0号策略规则

匹配任何条件，查询路由表local，该表local是一个特殊的路由表，包含对于本地和广播地址的优先级控制路由。

0号策略规则非常特殊，不能被删除或者覆盖。所有的数据包去 local路由表中去查找自己的路由方向，没有找到则使用下一优先级的路由策略。

* 32766号策略规则

匹配任何条件，查询路由表main表，该表是一个通常的表，包含所有的无策略路由，系统管理员可以删除或者使用另外的规则覆盖这条规则。

* 32767号策略规则

匹配任何条件，查询路由表default，该表是一个空表，它是后续处理保留，对于前面的策略没有匹配到的数据包，

系统使用这个策略进行处理，这个规则也可以删除。

6、来看一个实例：公司内网要求192.168.0.100 以内的使用 10.0.0.1 网关上网 （电信），其他IP使用 20.0.0.1 （网通）上网

a、先设置一个默认的路由，都指向 20.0.0.1

```shell
route add default gw 20.0.0.1
```

b、新建一个路由表3，里面增加一条路由，指向 10.0.0.1

```shell
ip route add table 3 default via 10.0.0.1 dev eth0
#查看table 3路由表，如果不加table就是查看默认的table 254表
ip route show table 3
```

c、新建一个规则，有某个特征（标记为3）的数据包，使用路由表3

```shell
ip rule add fwmark 3 table 3
#查看所有路由规则，可以看到fwmark 3是标记，table 3是路由表3上边。意思就是凡是标记了3的数据包使用table3路由表
ip rule
```

d、使用 iptables 给相应的数据打上标记

```shell
iptables -A PREROUTING -t mangle -i eth0 -s 192.168.0.1 - 192.168.0.100 -j MARK --set-mark 3
```

e、刷新路由表缓存

```shell
ip ro flush cache
```



### ipset介绍

1、解决什么问题？

考虑下面的场景，禁止某个IP地址【192.168.72.1】访问，我们使用iptables命令如下：

```shell
iptables -A INPUT -s 192.168.72.1 -j DROP
```

2、如果我还要禁止其它很多IP地址访问，而且没有啥规律，怎么办？
笨办法就是：

```shell
iptables -A INPUT -s aaa -j DROP
iptables -A INPUT -s bbb -j DROP
...
```

3、这样写有两个问题：维护困难和匹配性能差

4、有没有更好的办法呢？

使用ipset，思路是：ipset设置一个组，iptables关联组名，ipset对组内成员维护

5、实例如下：

```shell
#新建组
ipset create blacklist hash:ip
#禁止组访问
iptables -I INPUT -m set --match-set blacklist src -j DROP
#维护组内成员
ipset add blacklist 192.168.72.1
```

**上面的src代表源ip匹配blacklist中的ip**

6、ipset可以对ip分组，也可以对mac、ip、port，以及组合进行分组，比如

```shell
ipset create blacklist hash:ip,port
```

**在数据结构中存在两个及以上的成员时，创建iptables策略时，就要用多个src或dst了，用逗号隔开**，比如

```shell
iptables -I INPUT -m set --match-set blacklist src,src -j DROP
```

这里第一个src代表源ip匹配blacklist中的ip，第二个src代表源端口匹配blacklist中的port

ipset保存组内成员，可以使用hash、bitmap、list等数据结构，比如

```shell
ipset create blacklist list:ip,port
```

7、为什么要使用不同的数据结构？

原因是：不同场景下，需要选择对应的数据结构，提高匹配效率







## 参考链接

[iptables 防火墙中端口的开放和关闭 开放80端口示例_但行益事莫问前程的博客-CSDN博客](https://blog.csdn.net/javahelpyou/article/details/114605008)

[SNAT和DNAT的原理以及实验_snat测试方法_复苏者的博客-CSDN博客](https://blog.csdn.net/m0_52904076/article/details/116022575)

[透明代理入门 | Project X (xtls.github.io)](https://xtls.github.io/Xray-docs-next/document/level-2/transparent_proxy/transparent_proxy.html#透明代理的实现)

[iptables的四表五链与NAT工作原理 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/347754874)

[一文带你学会使用ipset - 简书 (jianshu.com)](https://www.jianshu.com/p/48fdaddd1383)

[用一个实例深入理解iptables的SNAT/DNAT - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/429294272)

[Linux防火墙配置工具iptables中MASQUERADE的含义_iptables masquerade-CSDN博客](https://blog.csdn.net/weixin_61637506/article/details/122072269)