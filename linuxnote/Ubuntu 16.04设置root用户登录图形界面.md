# Ubuntu 16.04设置root用户登录图形界面

Ubuntu默认的是root用户不能登录图形界面的，只能以其他用户登录图形界面。这样就很麻烦，因为权限的问题，不能随意复制删除文件，用gedit编辑文件时经常不能保存，只能用vim去编辑。

下面以Ubuntu16.04版为例说明：

1. 打开终端
2. 输入`sudo vi /usr/share/lightdm/lightdm.conf.d/50-ubuntu.conf`
3. 添加一行:greeter-show-manual-login=true保存关闭
4. 给root设置密码,在中端中输入:`sudo passwd root`
5. 如果现在用root用户在图形界面登录会有错误,读取/root/.profile时发生错误:mesg:tty n,还需要做如下修改:`sudo vi /root/.profile`,找到mesg n替换成tty -s && mesg n
6. 重启系统就能以root身份登录图形界面了



参考链接:

[Ubuntu 16.04设置root用户登录图形界面](https://www.linuxidc.com/Linux/2017-01/139094.htm)





