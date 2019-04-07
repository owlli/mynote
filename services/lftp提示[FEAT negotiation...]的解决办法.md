# lftp提示[FEAT negotiation...]的解决办法



ubuntu18.04上的lftp v4.8.1,连接centos7.5上的vsftpd v3.0.2,登录后执行任何命令都提示`[FEAT negotiation...]`卡死.

而无论是centos7.5上的lftp v4.4.8登录自己的vsftpd还是ubuntu18.04上的lftp登录自己的vsftpd v3.0.3,还是网页登录ftpd,都没有任何问题.百度找不到可以完全解决的办法,最后还是通过谷歌找到了.哎,真坑.

## 解决办法

修改`/etc/lftp.conf`或者`~/.config/lftp/rc`(没有就新建,根据`man lftp`的FILES节,修改` ~/.lftp/rc`,`~/.lftprc`同理)

修改下列选项,如果没有就新增(没修改过上述文件的应该是没有下列选项):

> set ftp:use-feat no                                                                                                 set ftp:ssl-allow no

## 推测

出现这个问题应该和lftp的主动模式被动模式都无关(一开始理解错了,百度关键词没找到解决办法后往这方面想,结果浪费了大量时间).应该是ubuntu18.04的lftp连接centos7.5上的vsftpd时默认使用了ssl证书加密,关闭这个选项就可以了