#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <linux/netfilter_ipv4.h>
#define PORT 2223		//端口号
#define BACKLOG 5	//最大监听数
 
int main()
{
	int iSocketFD = 0;  //socket句柄
	int iRecvLen = 0;   //接收成功后的返回值
	int new_fd = 0; 	//建立连接后的句柄
	char buf[4096] = {0}; //
	int n = 0;
	int ret = 0;
	struct sockaddr_in stLocalAddr = {0}; //本地地址信息结构图，下面有具体的属性赋值
	struct sockaddr_in stRemoteAddr = {0}; //对方地址信息
	socklen_t socklen = 0;  
 
	iSocketFD = socket(AF_INET, SOCK_STREAM, 0); //建立socket
	if(0 > iSocketFD)
	{
		printf("创建socket失败！\n");
		return 0;
	}	

	int val = 1;
	// 进程关闭后可立即重复使用这个ip、端口的socket
	if(setsockopt(iSocketFD, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0)
	{
		perror("setsockopt(), SO_REUSEADDR");
		return 0;
	}
	// 透明代理选项，支持伪造ip
	// 这里不加这个选项也能正常用，但不会匹配到-m socket --transparent iptables规则
	if(setsockopt(iSocketFD, SOL_IP, IP_TRANSPARENT, &val, sizeof(val)) < 0)
	{
		perror("setsockopt(), TRANSPARENT");
		return 0;
	}


	stLocalAddr.sin_family = AF_INET;  /*该属性表示接收本机或其他机器传输*/
	stLocalAddr.sin_port = htons(PORT); /*端口号*/
	stLocalAddr.sin_addr.s_addr=htonl(INADDR_ANY); /*IP，括号内容表示本机IP*/
 
	//绑定地址结构体和socket
	if(0 > bind(iSocketFD, (void *)&stLocalAddr, sizeof(stLocalAddr)))
	{
		printf("绑定失败！\n");
		perror("bind");
		return 0;
	}
 
	//开启监听 ，第二个参数是最大监听数
	if(0 > listen(iSocketFD, BACKLOG))
	{
		printf("监听失败！\n");
		return 0;
	}
 
	printf("iSocketFD: %d\n", iSocketFD);	
	//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小
	while(1)
	{
		socklen = sizeof(stRemoteAddr);
		new_fd = accept(iSocketFD, (void *)&stRemoteAddr, &socklen);
		printf("new_fd: %d\n", new_fd);	
		if(0 > new_fd)
		{
			printf("接收失败！\n");
			return 0;
		}else{
			printf("接收成功！\n");
			char ipv4[32] = {0, };
			char *src = NULL;
			struct sockaddr_in stClientAddr = {0}; //客户端地址信息
			stClientAddr = stRemoteAddr;
			src = inet_ntop(AF_INET, &stClientAddr.sin_addr, ipv4, sizeof(ipv4));
			if (!src) 
			{
				perror("inet_ntop error");
				return 0;
			}
			printf("client address %s:%hu\n", ipv4, ntohs(stClientAddr.sin_port));	

			n = sizeof(struct sockaddr_in);
			ret = getsockopt(new_fd, SOL_IP, SO_ORIGINAL_DST, &stRemoteAddr, &n);
			if(0 != ret)
			{
				printf ("error getting original destination address.\n");
				close (new_fd);
				return -1;
			}
			stRemoteAddr.sin_family = AF_INET; 
			src = inet_ntop(AF_INET, &stRemoteAddr.sin_addr, ipv4, sizeof(ipv4));
			if (!src) 
			{
				perror("inet_ntop error");
				return 0;
			}
			// printf("original destination address %u:%hu\n", ntohl(stRemoteAddr.sin_addr.s_addr), ntohs(stRemoteAddr.sin_port));	
			printf("original destination address %s:%hu\n", ipv4, ntohs(stRemoteAddr.sin_port));	
			//发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
			recv(new_fd, buf, sizeof(buf), 0);
			printf("从客户端上接收到的信息是:%s\n", buf);
			//send(new_fd, "这是代理服务器接收成功后发回的信息!", sizeof("这是代理服务器接收成功后发回的信息!"), 0);
 
			/* 连接服务器*/
			int iSockClientFD = 0;
			iSockClientFD = socket(AF_INET, SOCK_STREAM, 0);
			if(0 > iSockClientFD)					
			{
				printf("代理向服务器建立连接失败！\n");
				perror("socket");
				return 0;
			}
			printf("建立连接服务器的socket成功！\n");

			int val = 1;
			// 透明代理选项，支持伪造ip
			if(setsockopt(iSockClientFD, SOL_IP, IP_TRANSPARENT, &val, sizeof(val)) < 0)
			{
				perror("setsockopt(), TRANSPARENT");
				return 0;
			}
			// bind到客户端的ip，伪造客户端连接服务端
			stClientAddr.sin_port = htons(0); /*随机找一个未使用的端口作为伪造的客户端端口*/
			if(0 > bind(iSockClientFD, (void *)&stClientAddr, sizeof(stClientAddr)))
			{
				printf("绑定失败！\n");
				perror("bind");
				return 0;
			}
			printf("socket bind 成功\n");

			if(0 > connect(iSockClientFD, (void *)&stRemoteAddr, sizeof(stRemoteAddr)))
			{
				printf("代理向服务器建立连接失败！\n");
				perror("connect");
				return 0;
			}
			printf("socket connect 成功\n");
			send(iSockClientFD, buf, sizeof(buf), 0);//向服务器发送消息
			recv(iSockClientFD, buf, sizeof(buf), 0);//接收来自服务器的消息
			printf("从服务器端收到的消息：%s\n", buf);//打印接收到的来自服务器的消息
			send(new_fd, buf, sizeof(buf), 0);//向客户端发送消息		
			close(new_fd);
			sleep(5);	
			close(iSockClientFD);
		}
	}
	close(new_fd);
	close(iSocketFD);
 
	return 0;
}
