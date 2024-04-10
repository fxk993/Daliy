#include <iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<cstdio>
using namespace std;
int main()
{
    int socksrv = socket(AF_INET, SOCK_STREAM, 0);
    char on = 1;
    string a;
    cin >> a;
    setsockopt(socksrv, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    struct sockaddr_in add, srv;
    add.sin_addr.s_addr = inet_addr("192.168.175.129");
    add.sin_family = AF_INET;
    add.sin_port = htons(8000);
    if (-1 == bind(socksrv, (struct sockaddr*)&add, sizeof(struct sockaddr)))
    {
        cout << "bind error!" << endl;
        return -1;
    }
    int len = sizeof(struct sockaddr_in);
    getsockname(socksrv, (struct sockaddr*)&srv, (socklen_t*)&len);
    cout << "ip:" << srv.sin_addr.s_addr << "\n" << "port:" << srv.sin_port << endl;
    listen(socksrv, 5);
    struct sockaddr_in addrclient;
    int lenc = sizeof(sockaddr_in*);
    while (1)
    {
        cout << "---------------wait client--------------\n";
        int sockcon = accept(socksrv, (struct sockaddr*)&addrclient, (socklen_t*)&lenc);
        char b[100] = "welcome your serv!";
        send(sockcon, b, 100,0);
        char recvbuf[100];
		int res=1;
while(res!=0){
        res=recv(sockcon, recvbuf, 100, 0);
cout<<"接受到"<<sizeof(recvbuf)<<"数据:"<<recvbuf<<endl;
			}
        cout << "close the client" << endl;
        close(sockcon);
        cout << "continue the listen?" << endl;
        char a;
        a=getchar();
	cin>>a;
        if (a != 'y')
            break;

    }
    close(socksrv);


   
    return 0;
}
