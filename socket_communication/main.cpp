#include <cstdio>
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

    //绑定端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;  //地址族协议
    saddr.sin_port = htons(9999); //小端转大端
    saddr.sin_addr.s_addr = INADDR_ANY; //任意一个IP
    int ret = bind(fd, (struct sockaddr*)&saddr, sizeof(saddr));//绑定
    if (ret == -1)
    {
        perror("bind");
        return -1;
    }

    //监听
    ret = listen(fd, 128);  //监听
    if (ret == -1)
    {
        perror("listen");
        return -1;
    }

    //和客户端建立连接
    struct sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    int cfd = accept(fd, (struct sockaddr*)&caddr, &len);  //接收客户端连接，阻塞函数
    if (cfd == -1)
    {
        perror("accept");
        return -1;
    }

    //打印一下客户端信息
    char ip[32];
    printf("client IP:%s  client port:%d\n", inet_ntop(AF_INET, &saddr.sin_addr.s_addr, ip, sizeof(ip)), ntohs(caddr.sin_port));//打印客户端信息
    //大端转小端
    
    
    //与客户端进行通信
    while (1)
    {
        char buff[1024];
        int len = recv(cfd, buff, sizeof(buff), 0);//接收客户端数据

        if (len > 0)
        {
            printf("client says:%s\n", buff);
            send(cfd, buff, strlen(buff) + 1, 0); //再把数据发送回去

        }
        else if (len == 0)
        {
            printf("client disconnected");
            break;
        }
        else {
            perror("recv");
        }
    }

    return 0;
}