# KVM虚拟机相关操作

## 说明

此篇笔记中虚拟机名用aaa、bbb之类字符代替



## 查看

```shell
# 查看所有已运行的虚拟机
virsh list
# 查看所有已安装的虚拟机
virsh list --all
# 查看虚拟机配置信息
virsh dominfo aaa
```



## 开机和关机

```shell
# 开机
virsh start aaa
# 关机
virsh shutdown aaa
# 强制关机，注意，强制关机容易损坏虚拟机文件系统，下次启动时可能要修复文件系统
virsh destroy aaa
```



## 强制关机导致磁盘损坏，修复磁盘

```shell
# 开机时检测到磁盘损坏，无法进入系统，以xfs文件系统修复举例
xfs_repair -L /dev/mapper/centos-root
```



## 调整虚拟机内存和cpu

```shell
# 关机
virsh shutdown aaa
# 查看虚拟机信息
virsh dominfo aaa
cpu信息在CPU(s)字段
内存信息在Max memory、Used memory字段
# 编辑配置信息
virsh edit aaa
内存设置在memory、current memory两个标签
cpu设置在vcpu标签
# 重新定义使配置文件生效
virsh define /etc/libvirt/qemu/aaa.xml
# 启动虚拟机
virsh start aaa
# 再次查看虚拟机信息
virsh dominfo aaa
可以看到cpu信息和内存信息发生了更改
```



## 删除虚拟机

```shell
# 检查虚拟机使用的磁盘文件，磁盘文件在disk标签
virsh dumpxml aaa
# 关闭虚拟机
virsh shutdown aaa
# 查看虚拟机有没有做过快照
virsh snapshot-list --domain aaa
# 如果有快照，先删除快照，假如快照名是123
virsh snapshot-delete --domain vm-name --snapshotname 123
# 删除虚拟机
virsh undefine aaa
# 虚拟机虽然被kvm删除，但磁盘文件还在，磁盘文件在第一步时已查到
rm -f /var/lib/libvirt/images/aaa.img
```



## 为虚拟机增加网卡

```shell
# 查看虚拟机的网卡
virsh domiflist aaa
# 1
# 临时增加网卡，重启虚拟机失效，--type指定使用桥接类型，--source指定和实体机的virbr0桥接在一起
virsh attach-interface aaa --type bridge --source virbr0
# 2
# 通过修改配置文件永久增加网卡
virsh attach-interface aaa --type bridge --source virbr0 --config
# 查看配置文件可以看到多出来一块网卡（看interface标签）
virsh edit aaa
# 删除永久增加的网卡
virsh detach-interface aaa --type bridge --mac 网卡mac地址 --config
```



## 克隆虚拟机

```shell
# 先把原虚拟机关机
virt-clone --original 原虚拟机名 --name 新虚拟机名 --file 新虚拟机磁盘文件路径

# 克隆虚拟机后，设备的网卡名可能会发生变化，绑定网卡名
vim /etc/udev/rules.d/70-persistent-net.rules
# 修改里面mac地址和网卡名的关系，如果没有内容，手动加入下面内容，将ATTR{address}值改为网卡mac地址，NAME改为网卡名
SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="52:54:00:26:c8:8e", ATTR{type}=="1", KERNEL=="eth*", NAME="eth0"
```



## 快照管理

```shell
# 创建快照前需要先关闭虚拟机aaa
virsh shutdown aaa
# 创建快照名为xxx
virsh snapshot-create-as aaa xxx
# 查看aaa的快照
virsh snapshot-list aaa
# 查看aaa的xxx快照信息
virsh snapshot-info aaa xxx
# 恢复到快照xxx
virsh snapshot-revert aaa xxx
# 删除快照
virsh snapshot-delete aaa xxx
```







