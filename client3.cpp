#include<stdio.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<error.h>
int main()
{
	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(c_addr));
	c_addr.sin_addr.s_addr = inet_addr("192.168.175.130");
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(8000);
	char buf[128] = { 0 };
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("socket error\n");
		return 0;
	}
	if (connect(sockfd, (struct sockaddr*)&c_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("connect error\n");
		return 0;
	}

	char msg[] = "3hao:nishishabi";
	while (1)
	{
		write(sockfd, msg, strlen(msg) + 1);
		/*read(sockfd, buf, sizeof(buf));
		printf("%s\n", buf);
		sleep(10);*/
	}

	close(sockfd);
	return 0;
}