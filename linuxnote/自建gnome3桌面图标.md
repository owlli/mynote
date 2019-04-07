# 自建gnome3桌面图标



## 1.创建desktop文件

以electronic开发的微信为例说明,新建一个名为`wechat.desktop`的文件,内容如下:

>[Desktop Entry]
>
>Name=Wechat
>
>Comment=wechat
>
>Type=Application
>
>Exec=/opt/electronic-wechat-linux-x64/electronic-wechat
>
>Icon=/opt/electronic-wechat-linux-x64/resources/wechat.png
>
>Terminal=false
>
>StartupNotify=true
>
>Categories=Application;

文件的参数解释如下

| **行**                       | **解释**                                                     |
| ---------------------------- | ------------------------------------------------------------ |
| [Desktop Entry]              | 每个桌面项文件的第一行和每节的开头,标识与桌面集成的键值对块,是桌面正确识别文件所必须的 |
| Type=Application             | 文件类型                                                     |
| Encoding=UTF-8               | 描述此桌面文件中的编码                                       |
| Name=Sample Application Name | 主菜单和任何启动器中您的应用程序的名称                       |
| Comment=A sample application | 描述这个应用程序。使用的是工具提示                           |
| Exec=application             | 从 shell 中启动此应用程序的命令,可以有参数                   |
| Icon=application.png         | 与此应用程序关联的图标名                                     |
| Terminal=false               | 说明程序是否在终端中运行                                     |

 Exec行可使用如下参数

| **添加...** | **接受...**                                                  |
| ----------- | ------------------------------------------------------------ |
| %f          | 单个文件名                                                   |
| %F          | 多个文件名                                                   |
| %u          | 单个 URL                                                     |
| %U          | 多个 URL                                                     |
| %d          | a single directory. Used in conjunction with %f to locate a file. |
| %D          | multiple directories. Used in conjunction with %F to locate files. |
| %n          | 不带路径的单个文件名                                         |
| %N          | 不带路径的多个文件名                                         |
| %k          | 用来表示桌面项文件位置的 URI 或本地文件名                    |
| %v          | 设备项的名称                                                 |



## 2.放到指定目录

* 放到`/usr/share/applications/`目录,会出现在桌面的应用列表里,对所以用户有效
* 放到`~/.local/share/applications`目录,会出现在桌面的应用列表里,只会对当前用户有效
* 放到`~/Desktop`目录(中文环境是`~/桌面`目录),会出现在当前用户的桌面,需要给它执行权限才能双击打开,第一次打开时会问是否信任,点信任就可以了