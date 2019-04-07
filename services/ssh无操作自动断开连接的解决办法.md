# ssh无操作自动断开连接的解决办法



## 方法一:服务器端修改

`vim /etc/ssh/sshd_config`

找到下面两行(如果没有,自行添加)

```shell
#ClientAliveInterval 0
#ClientAliveCountMax 3
```

去掉注释,并设置值为:

```shell
#单位为秒,表示每10分钟向客户端发送一次消息
ClientAliveInterval 600
#表示服务器发出请求后客户端无响应的次数为5后,自动断开ssh连接
ClientAliveCountMax 5
```

## 方法二:客户端修改

`vim /etc/ssh/ssh_config`

添加下面两行:

```shell
#单位为秒,表示每10分钟向客户端发送一次消息
ServerAliveInterval 60
#表示服务器发出请求后客户端无响应的次数为5后,自动断开ssh连接
ServerAliveCountMax 10 
```





