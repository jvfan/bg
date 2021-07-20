#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/*外部声明需要被调用api函数*/
#include "include/socket_api.h"

/*-----------------------------------------------------------------*/
/*------------TCP------------*/
//server端
//建立TCP监听,返回服务端句柄，不读写它
int tcp_init(char *host_ip,unsigned short portvalue,unsigned short numvalue){//传入本地ip
	int ret;
	//服务器变量
	int sockfd;//socket的句柄
	struct sockaddr_in addr;//socket的地址端口，ipv4用这个结构体，unix socket用sockaddr_un
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket's fd is %d!\n",sockfd);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portvalue); //存储中有大小端存储，这里转换为统一顺序
	addr.sin_addr.s_addr = inet_addr(host_ip);

	ret = bind(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	if(ret == -1)
	{
		perror("bind");
		exit(-1);
	}
	
	ret = listen(sockfd, numvalue);//间接决定最大连接数，开始监听
	if(ret == -1)
	{
		perror("listen");
		exit(-1);
	}
	return sockfd;
}
//server端
//建立TCP连接，返回客户端句柄，读写它（recv、send）
//如果有多个客户端，只需while(1)下面的部分以及读写,以及使用多线程
int tcp_build(int sockfd){//传入已监听的sockfd
	int clientfd;//client socket的句柄
	struct sockaddr_in peer;//客户端地址信息保存的结构体
	socklen_t peer_len = sizeof(struct sockaddr_in);//客户端结构体大小
	char ipbuf[64];//存放客户端的ip字符串
	
	//获取连接的客户端地址信息，该函数会阻塞！！！！！
	clientfd = accept(sockfd,(struct sockaddr *)&peer,&peer_len);
	printf("client socket's fd is %d!\n",clientfd);
	//注意，下面inet_ntop函数中的参数ipbuf是必要的，不能为NULL，存放ip字符串
	printf("client ip is %s:%d\n", inet_ntop(AF_INET,&peer.sin_addr,ipbuf,64), ntohs(peer.sin_port));
	return clientfd;
}

//client端
//请求TCP连接，返回本地句柄，写读它（write、read）
int tcp_request(char *server_ip,unsigned short portvalue){//传入服务器ip
	int ret;
	int sockfd;//socket的句柄
	struct sockaddr_in addr;//socket的地址端口，ipv4用这个结构体，unix socket用sockaddr_un

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	printf("socket's fd is %d!\n",sockfd);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(portvalue); //存储中有大小端存储，这里转换为统一顺序
	addr.sin_addr.s_addr = inet_addr(server_ip);
	
	ret = connect(sockfd,(struct sockaddr *)&addr,sizeof(addr));
	if(ret == -1){
		perror("connect");
		exit(-1);
	}
	return sockfd;
}

/*-----------------------------------------------------------------*/
/*------------UDP------------*/
//先接收端，需要bind，作为服务端
//补充UDP信息，并且bind，读写它
int udp_init_firstrecv(struct sockaddr_in servaddr,char *host_ip,unsigned short portvalue){//传入本地ip
	int sockfd;
	
	sockfd = socket(AF_INET, SOCK_DGRAM , 0);
	printf("socket's fd is %d!\n",sockfd);
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(host_ip);//可填"0.0.0.0"，指所有本机ip
	servaddr.sin_port = htons(portvalue); //端口，存储中有大小端存储，这里转换为统一顺序
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));//绑定
	return sockfd;
}

//先发送端，不需要bind，作为客户端
//补充UDP信息，写读它
int udp_init_firstsend(struct sockaddr_in *servaddr,char *server_ip,unsigned short portvalue){//传入远程ip
	int sockfd;
	
	sockfd = socket(AF_INET, SOCK_DGRAM , 0);
	printf("socket's fd is %d!\n",sockfd);

	servaddr->sin_family = AF_INET;
	servaddr->sin_addr.s_addr = inet_addr(server_ip);
	servaddr->sin_port = htons(portvalue); //端口，存储中有大小端存储，这里转换为统一顺序

	return sockfd;
}

//udp接收和保存远程地址信息
ssize_t udp_recv(int sockfd,char *buf,size_t buf_size,struct sockaddr_in *addr){//传入句柄，保存远程ip
	socklen_t addr_len = sizeof(struct sockaddr_in); //记录来源地址的变量
	return recvfrom(sockfd, buf, buf_size, 0, (struct sockaddr *)addr, &addr_len);
}

//udp发送到远程
ssize_t udp_send(int sockfd,char *buf,struct sockaddr_in *addr){//传入句柄和远程ip
	return sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
}

