# SElinux

SElinux是一种强制访问控制



## 查看SElinux的状态

```shell
getenforce
sestatus
```



## 临时关闭SElinux

```shell
setenforce 0
```



## 临时开启SElinux

```shell
setenforce 1
```



## 永久关闭SElinux

```shell
vim /etc/selinux/config
#将SELINUX=enforcing改为SELINUX=disabled，重启机器即可
#/etc/selinux/config有个链接文件，是/etc/sysconfig/selinux
```

