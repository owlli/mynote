# linux系统时间



## 查看系统时间

```shell
date
```

查看日历

```shell
cal
```



## 时区相关

查看当前时区

```shell
date -R
#或者
ls -l /etc/localtime
```

修改当前时区

```shell
tzselect
#适用于RedHat系
timeconfig
#适用于Debian系
dpkg-reconfigure tzdata
#复制相应的时区文件，替换系统时区文件；或者创建链接文件
ln -s /usr/share/zoneinfo/$主时区/$次时区 /etc/localtime
```



## 修改系统时间

```shell
#改年月日
date -s 月/日/年
#改时分秒
date -s 时:分:秒
```



## 硬件(bios)时间

下面操作都需要root权限

```shell
#显示硬件时间
hwclock -r
#修改系统时间为硬件时间
hwclock -s
#修改硬件时间为系统时间
hwclock -w
```









