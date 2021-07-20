#ifndef __SOCKET_API_H
#define __SOCKET_API_H

#include <netinet/in.h>

/*------tcp------*/
//server
extern int tcp_init(char *host_ip,unsigned short portvalue,unsigned short numvalue);
extern int tcp_build(int sockfd);
//client
extern int tcp_request(char *server_ip,unsigned short portvalue);

/*------udp------*/
//first recv port,first run,server
extern int udp_init_firstrecv(struct sockaddr_in servaddr,char *host_ip,unsigned short portvalue);
//first send port,last run,client
extern int udp_init_firstsend(struct sockaddr_in *servaddr,char *server_ip,unsigned short portvalue);
extern ssize_t udp_recv(int sockfd,char *buf,size_t buf_size,struct sockaddr_in *addr);//udp接收
extern ssize_t udp_send(int sockfd,char *buf,struct sockaddr_in *addr);//udp发送

#endif

/* how to use

  <!----------TCP部分---------->
  
  tcp服务器demo:
  ==================================
	int tcp_server_fd,tcp_client_fd;
	struct sockaddr_in servaddr,addr;
	char buf[64];

	if((tcp_server_fd = tcp_init("0.0.0.0",55005,5)) < 0){
		return -1;
	}

	tcp_client_fd = tcp_build(tcp_server_fd);
	while(1){
		memset(buf,0,sizeof(buf));
		read(tcp_client_fd,buf,64);
		if(strncmp(buf,"exit",4) == 0){//输入exit退出
			close(tcp_client_fd);
			break;
		}
		printf("%s",buf);
		write(tcp_client_fd,"hallo",6);
	}

	close(tcp_server_fd);
  ==end=============================
  
  tcp客户端demo:
  ==================================
	int tcp_fd;
	char str[64];
	
	if((tcp_fd = tcp_request("192.168.2.115",10500)) < 0){
		return -1;
	}
	
	while(1){
		memset(str,0,sizeof(str));
		scanf("%s",str);
		fflush(stdin);
		if(strncmp(str,"exit",4) == 0){//输入exit退出
			close(tcp_fd);
			break;
		}
		write(tcp_fd,str,strlen(str));
	}
  ==end=============================

  tcp服务端两进程一收一发demo:
  ==================================
	pid_t pid;
	int tcp_server_fd,tcp_client_fd;
	char str[64],buf[64];

	if((tcp_server_fd = tcp_init("0.0.0.0",55005,5)) < 0){
		return -1;
	}
	
	tcp_client_fd = tcp_build(tcp_server_fd);
	
	pid = fork();
	if(pid == -1){
		printf("fork failed\n");
		return 1;
	}
	else if(pid){ //当前进程（父进程）可输入
		while(1){
			memset(str,0,sizeof(str));
			scanf("%s",str);
			fflush(stdin);
			if(strncmp(str,"exit",4) == 0){//输入exit退出
				close(tcp_client_fd);
				kill(pid,9);
				break;
			}
			write(tcp_client_fd,str,strlen(str));
		}
	}
	else{ //子进程
		while(1){
			memset(buf, 0, sizeof(buf));
			read(tcp_client_fd, buf, sizeof(buf));
			printf("socket data is %s!\n",buf);//至少加\n，否则有可能打印不出来
		}
	}
	
	close(tcp_client_fd);
	close(tcp_server_fd);
  ==end=============================
  
  <!----------UDP部分---------->
  
  udp先收demo:
  ==================================
	int udp_fd;
	struct sockaddr_in servaddr,src_addr;//src_addr记录来源地址
	char buf[64];
	
	if((udp_fd = udp_init_firstrecv(servaddr,"0.0.0.0",55006)) < 0){
		return -1;
	}
	
	udp_recv(udp_fd,buf,&src_addr);//接收并记录来源信息
	printf("%s",buf);
  ==end=============================
  
  udp先发demo:
  ==================================
	int udp_fd;
	struct sockaddr_in servaddr;//socket的地址端口，ipv4用这个结构体
	
	if((udp_fd = udp_init_firstsend(&servaddr,"192.168.2.115",10500)) < 0){
		return -1;
	}
	
	udp_send(udp_fd,"hello world",&servaddr);
	close(udp_fd);
  ==end=============================
*/