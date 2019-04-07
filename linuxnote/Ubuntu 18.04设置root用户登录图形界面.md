# Ubuntu 18.04设置root用户登录图形界面



## 创建root密码

```shell
Sudo passwd
```

根据提示创建root密码



## 修改配置文件

```shell
 sudo vim /etc/pam.d/gdm-autologin
```

将

> auth   required        pam_succeed_if.so user != root quiet_success

注释

```shell
sudo vim sudo vim /etc/pam.d/gdm-password
```

将

> auth   required        pam_succeed_if.so user != root quiet_success

注释

```shell
 sudo vim /root/.profile
```

将

> mesg n || true

行改为

> tty -s && mesg n || true



### 重启

