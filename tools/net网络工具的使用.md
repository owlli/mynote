# net网络工具的使用



## ethtool

-p/--identify 网卡名 [N]	点亮网卡上的灯,后面可以加点亮时间,没加就是一直亮

-k 网卡名	查看网络接口的offload特性

-K 网卡名 gro/tso/gso/tso on/off	打开关闭网卡的某个特性





## 调整pcap文件中顺序

如果一个名为in.pcap的包中的序号如下:

1,2,3,4,5,6

想调整成:

2,3,1,4,5,6

可以使用`editcap`和`mergecap`这两个工具,方法如下:

```shell
editcap -r in.pcap tmp1 2-3
editcap -r in.pcap tmp2 1
editcap -r in.pcap tmp3 4-6
mergecap -w out.pcap -a tmp1 tmp2 tmp3 
```















