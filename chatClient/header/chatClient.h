#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <iostream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

//包含信息类头文件
#include "MSG.h"

//使用标准命名空间
using namespace std;

#define BUFFER_SIZE 512

//定义聊天室客户端类
class chatClient
{
private:
    int cfd; //定义客户端用于通信的套接字
    string name; //定义客户端的用户名
    bool recvRunning; //定义客户端接收线程是否运行的标志

    //向服务器发送消息
    void Send_Message(int type,const string content = "");
    
    //接收服务器发来的消息
    void Recv_Message();

public:

    chatClient(const char* ip,int port,string name);
    ~chatClient();

    //运行客户端
    void Run_ChatClient();
};





#endif
