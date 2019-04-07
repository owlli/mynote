# 查看linux硬件信息



## 查看序列号

```shell
dmidecode -s system-serial-number
#或者
dmidecode -t1
#此命令的Serial Name行既是序列号
```



## 查看服务器型号

```shell
dmidecode |grep -i product
```



