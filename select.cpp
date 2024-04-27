#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<error.h>
#include<stdio.h>
#include<unistd.h>
int main()

{
	sockaddr_in saddr, daddr;
	saddr.sin_addr.s_addr = inet_addr("192.168.175.130");
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8000);
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("sock fail\n");
		return 0;
	}
	if (-1 == bind(lfd, (sockaddr*)&saddr, sizeof(saddr)))
	{

		perror("bind error\n");
		return 0;
	}
	listen(lfd, 128);
	fd_set readset;
	fd_set tempset;
	FD_ZERO(&readset);
	FD_ZERO(&tempset);
	int maxfd = lfd;
	FD_SET(lfd, &readset);
	socklen_t len = sizeof(daddr);
	char recvbuf[128];
	while (1)
	{
		tempset = readset;
		select(maxfd + 1, &tempset, NULL, NULL, NULL);
		for (int i = 0; i < maxfd; i++)
		{
			if (FD_ISSET(i, &tempset))
			{
				if (i == lfd)

				{
					int cfd = accept(lfd, (sockaddr*)&daddr, &len);
					if (cfd == -1)
					{
						perror("accept error\n");
						return 0;
					}
					FD_SET(cfd, &readset);
					maxfd = cfd;
				}
				else
				{
					while (1) {
						int len = read(i, recvbuf, sizeof(recvbuf));
						if (len < 0)
						{
							perror("recv error\n");
							break;
						}
						else if (len == 0)
						{
							printf("client has disconnect\n");
							FD_CLR(i, &readset);
							close(i);
							break;
						}
						else
						{
							printf("recevie:%s bytes\n", recvbuf);
						}
					}

				}
			}
		}
	}
}