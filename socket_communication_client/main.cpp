#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>


int main()
{
    //创建套接字
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("socket");
		return -1;
	}

    //连接
	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(9999);
	inet_pton(AF_INET, "192.168.74.133", &saddr.sin_addr.s_addr);  //指定一个确切的IP
	int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));  //连接
	if (ret == -1)
	{
		perror("connect");
		return -1;
	}

    //通信
    int number = 0;
    while (1)
    {
        char buff[1024];
        sprintf(buff, "hello server%d\n", number++);  //写一个字符串
        send(fd, buff, sizeof(buff) + 1, 0);
        memset(buff, 0, sizeof(buff));
        int len = recv(fd, buff, sizeof(buff), 0);
        if (len > 0)
        {
            printf("server says:%s", buff);
        }
        else if (len == 0)
        {
            printf("disconnected");
            break;
        }
        else {
            perror("recv");
        }
        sleep(1);

    }
    //关闭套接字
    close(fd);
    return 0;
}