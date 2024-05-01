#include "http.h"
#include<sys/epoll.h>
#include<fcntl.h>
#include<unistd.h>

//对epoll的操作
void setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFD);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFD, new_option);
}
void addfd(int epollfd,int fd,bool one_shot,int trigmode)
{
	epoll_event event;
	event.data.fd = fd;
	if (trigmode == 1)
	{
		event.events = EPOLLIN | EPOLLHUP | EPOLLET;
	}
	else

	{
		event.events = EPOLLIN | EPOLLHUP;
	}
	if (one_shot)
	{
		event.events |= EPOLLONESHOT;
		
	}
	setnonblocking(fd);
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);

}
void removefd(int epollfd, int fd)

{
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
	close(fd);
}
void modfd(int epollfd, int fd, int ev, int trigmode)
{
	epoll_event event;
	event.data.fd = fd;
	if (trigmode == 1)
	{
		event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
	}
	else
	{
		event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
	}
	epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void http_conn::init(int sockfd, const sockaddr_in& addr, char*, int, int, string user, string password, string sqlname)
{
	m_sockfd = sockfd;
	m_addr = addr;
	addfd(m_epollfd)

}
void http_conn::close_conn(bool real_close)
{
	if (real_close && (m_sockfd != -1))
	{
		printf("close %d\n", m_sockfd);
		removefd(m_epollfd, m_sockfd);
		m_sockfd = -1;
		m_use_count--;
	}
}
http_conn::LINE_STATUS http_conn::parse_line()
{

	char temp;
}
