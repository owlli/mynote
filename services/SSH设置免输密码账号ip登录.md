# SSH设置免输密码账号ip登录



## 1.客户端配置

### 1.设置别名

编辑`.ssh/config`,根据服务器端sshd配置,新增内容如下

```shell
Host 别名
    Hostname 服务器名或ip
    Port 服务器sshd端口
    User 服务器端待登录的用户名
```

### 2.生成秘钥

执行下面命令,一直回车

```shell
ssh-keygen -t rsa
```

### 3.发送秘钥

执行下面命令,然后输入服务器端密码

```shell
ssh-copy-id 别名
```



## 2.服务器端配置

重启sshd服务



## 3.使用方法

以上配置好后,直接在客户端执行

```shell
ssh 别名
```

即可登录服务器