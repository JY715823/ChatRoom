#ifndef CHATSERVER_H
#define CHATSERVER_H

//包含库文件
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//包含线程池类头文件
#include "threadPool.h"
//包含信息类头文件
#include "MSG.h"

//使用标准命名空间
using namespace std;

#define THREADNUM 10
#define BUFFER_SIZE 512

//定义聊天室服务端类
class chatServer
{
private:
    //定义服务器的属性
    int sfd; // 定义服务端套接字
    bool running; // 定义服务器是否运行的标志
    
    //定义存储客户端的结构体和容器
    struct Client{
        int cfd; //用于和客户端通信的套接字
        struct sockaddr_in caddr; //客户端的地址信息
    };
    vector<Client> clients;

    //定义服务器用于高并发处理任务的线程池对象
    threadPool tPool;

    //处理客户端发来的信息
    void Handle_ClientMsg(int cfd,const struct sockaddr_in& caddr);
    //广播发送消息
    void Broadcast(string msg,int excludfd = -1);

public:
    //定义保护客户端容器的互斥锁
    mutex cliLock;

    chatServer(const char* ip,int port,int threadNums);
    ~chatServer();

    //启动服务器
    void Run_ChatServer();
};





#endif