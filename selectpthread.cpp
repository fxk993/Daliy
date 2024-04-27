//#include<unistd.h>
//#include<sys/select.h>
//#include<arpa/inet.h>
//#include<stdio.h>
//#include<sys/socket.h>
//#include<error.h>
//#include<string.h>
//#define MAX 100
//int main()
//{
//	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//	if (sockfd < 0)
//	{
//		perror("sock error");
//		return 0;
//	}
//	struct sockaddr_in ser_addr, cli_addr;
//	ser_addr.sin_addr.s_addr = inet_addr("192.168.175.130");
//	ser_addr.sin_port = htons(8000);
//	ser_addr.sin_family = AF_INET;
//	if (bind(sockfd, (sockaddr*)&ser_addr, sizeof(ser_addr)) == -1)
//	{
//		perror("bind error\n");
//	}
//	if (listen(sockfd, MAX) == -1)
//	{
//		perror("listen error\n");
//		return 0;
//	}
//	fd_set rdset, rdtmp;
//	FD_ZERO(&rdset);
//	FD_ZERO(&rdtmp);
//	FD_SET(sockfd, &rdset);
//	char buf[128];
//	while (1)
//	{
//		rdtmp = rdset;
//		int i = 0;
//		socklen_t clisize = sizeof(cli_addr);
//		select(MAX, &rdtmp, NULL, NULL, NULL);
//		for (i = 0; i <= MAX; i++)
//		{
//			if (FD_ISSET(i, &rdtmp))
//			{
//				if (i == sockfd)
//				{
//					int cfd = accept(sockfd, (sockaddr*)&cli_addr, (socklen_t*)&clisize);
//					FD_SET(cfd, &rdset);
//				}
//				else
//				{
//					int len = read(i, buf, sizeof(buf));
//					if (len > 0)
//					{
//						printf("receive:%s\n", buf);
//						sprintf(buf, "server has received your message");
//						sleep(10);
//						write(i, buf, strlen(buf)+1);
//					}
//					else if (len == 0)
//					{
//						printf("client has disconected\n");
//						FD_CLR(i, &rdset);
//						close(i);
//					}
//					else
//					{
//						perror("read error");
//					}
//				}
//			}
//		}
//	}
//	close(sockfd);
//	return 0;
//}
#include<sys/socket.h>
#include<sys/select.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<error.h>
#include<stdio.h>
#include<pthread.h>
typedef struct node
{
	int fd;
	fd_set* readset;
	int* maxfd;
}MFD;
void* m_connect(void* args)
{
	sockaddr_in daddr;
	MFD* m_lfd = (MFD*)args;
	socklen_t len = sizeof(sockaddr);
	int cfd = accept(m_lfd->fd, (sockaddr*)&daddr, &len);
	if (cfd == -1)
	{
		perror("accept fail\n");
		pthread_exit(NULL);
	}
	FD_SET(cfd, m_lfd->readset);
	m_lfd->maxfd = &cfd;
}
int main()
{
	sockaddr_in saddr;
	saddr.sin_addr.s_addr = inet_addr("192.168.175.130");
	saddr.sin_port = htons(8000);
	saddr.sin_family = AF_INET;
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("sock fail!");
		return 0;
	}
	if (bind(lfd, (sockaddr*)&saddr, sizeof(saddr)) == -1)
	{
		perror("bind error\n");
		return 0;

	}
	int maxfd = lfd;
	listen(lfd, 128);
	fd_set readset;
	fd_set tempset;
	FD_ZERO(&readset);
	FD_ZERO(&tempset);
	FD_SET(lfd, &readset);

	while (1)
	{
		MFD args;
		tempset = readset;
		select(maxfd + 1, &tempset, NULL, NULL, NULL);
		for (int i = 0; i < maxfd; i++)
		{
			if (FD_ISSET(i, &tempset) == 1)
			{
				if (i == lfd)
				{
					args.fd = lfd;
					args.readset = &readset;
					args.maxfd = &maxfd;
					pthread_t connectpid;
					pthread_create(&connectpid, NULL, m_connect, &args);

				}
				else
				{
					pthread_t communpid;
					pthread_create(&communpid, NULL, m_commun, )
				}
			}
		}
	}


}