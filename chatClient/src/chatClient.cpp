#include "chatClient.h"

//定义打印错误日志宏函数
#define ERR_LOG(msg) do{\
    cout << __FILE__ << "——" << __func__ << ":" << __LINE__ << endl; \
    perror(msg);\
}while(0)

chatClient::chatClient(const char* ip,int port,string name):recvRunning(true),name(name)
{
    //创建客户端用于通信的套接字
    this->cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd < 0){
        ERR_LOG("socket error");
        return;
    }

    //定义存储服务端地址信息结构体
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);
    //和服务器进行连接
    if(connect(cfd,(struct sockaddr*)&saddr,sizeof(saddr)) == -1){
        ERR_LOG("connect error");
        return;
    }

    //向客户端发送登录信息
    Send_Message(LOGIN);

}

chatClient::~chatClient()
{
    //关闭客户端用于通信的套接字
    if(cfd >= 0){
        close(cfd);
    }
}

//运行客户端
void chatClient::Run_ChatClient(){
    //创建线程用来接收服务器发来的消息，主线程用来向服务器发送消息
    thread recvThread(&chatClient::Recv_Message,this);

    //在终端输入信息发送到服务器
    string buf;
    while(true){
        getline(cin,buf);

        if(buf == "quit"){
            Send_Message(QUIT);
            //将接收消息的线程运行标志设置为停止
            recvRunning = false;
            break;
        }else{
            Send_Message(CHAT,buf);
        }
    }

    //等待接收信息线程结束
    recvThread.join();
}


//向服务器发送消息
void chatClient::Send_Message(int type,const string content){
    //定义信息结构体
    MSG msg;
    msg.type = htonl(type);
    strncpy(msg.name,this->name.c_str(),sizeof(msg.name));
    msg.name[sizeof(msg.name) - 1] = '\0';
    if(content.size() != 0){
        strncpy(msg.content,content.c_str(),sizeof(msg.content));
        msg.content[sizeof(msg.content) - 1] = '\0';
    }

    string buf = msg.Serialize();
#if 0
    cout << buf << endl;
#endif
    //向服务器发送信息
    if(send(cfd,buf.c_str(),buf.length(),0) == -1){
        ERR_LOG("send error");
        return;
    }
}

//接收服务器发来的消息
void chatClient::Recv_Message(){
    //定义缓冲区
    char buf[BUFFER_SIZE] = "";
    while(recvRunning){
        //接收服务器的信息
        int res = recv(cfd,buf,sizeof(buf),0);
        if(res <= 0){
            if(!recvRunning){
                break;
            }else{
                cout << "服务器已下线！" << endl;
                break;
            }
        }

        //解析服务器发来的消息
        MSG msg;
        msg.Deserialize(string(buf,res));
        //根据不同消息类型进行处理
        int t = ntohl(msg.type);
#if 0
        cout << t << " " << msg.name << " " << msg.content << endl;
#endif
        if(t == LOGIN){
            cout << "------ " << msg.name << "进入聊天室 ------" << endl;
        }else if(t == CHAT){
            cout << msg.name << ":" << msg.content << endl;
        }else if(t == QUIT){
            cout << "------ " << msg.name << "退出聊天室 ------" << endl;
        }
    }

}