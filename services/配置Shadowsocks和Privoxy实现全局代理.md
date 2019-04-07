# 配置Shadowsocks和Privoxy实现全局代理



## 1.已知问题

执行３.4步骤后,内网使用realvncviewer无法连接,ubuntu自带的Remmina可正常使用,取消定义这三个变量后realvncviewer可正常使用



##　2.安装shadowsocks并配置



## 3.配置privoxy

1. 安装

```shell
sudo apt-get install privoxy
```

2. 配置privoxy

```shell
sudo /etc/privoxy/config
```

找到4.1节的最后部分,确定配置如下:

>listen-address  127.0.0.1:8118

找到5.2节最后部分,确认配置如下

> forward-socks5 / 127.0.0.1:1080 .

(注意最后的点)

3. 重启

```shell
sudo systemctl restart privoxy.service
```

4. 设置变量

```shell
export http_proxy="127.0.0.1:8118"                                                  
export https_proxy=$http_proxy
export ftp_proxy=$http_proxy
```

可以把这些变量定义写在配置文件里开机自动设置



## 4.测试

可以使用

```shell
wget http://www.google.com 
```

或者

```shell
curl www.google.com
```

不要使用`ping`,可能是因为ss 不能代理 ping 使用的 icmp 协议