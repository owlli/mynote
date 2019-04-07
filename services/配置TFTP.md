# 配置TFTP



## 一、安装**tft**p-server
sudo apt-get install **tftp**d-hpa

## 二、创建tftp下载目录
sudo mkdir /tftpboot
sudo chmod 777 /tftpboot

## 三、配置TFTP服务器
sudo vim /etc/default/tftpd-hpa
写入以下内容：

TFTP_USERNAME="tftp"
TFTP_ADDRESS="0.0.0.0:69"
TFTP_DIRECTORY="/tftpboot"
TFTP_OPTIONS="-l -c -s"

## 四、重新启动TFTP服务
sudo service tftpd-hpa restart
或者
sudo /etc/init.d/tftpd-hpa restart

