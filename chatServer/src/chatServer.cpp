#include "chatServer.h"

//定义打印错误日志的宏函数
#define ERR_LOG(msg) do{\
    cerr << __FILE__ << ":" << __func__ << ":" << __LINE__ << endl;\
    perror(msg);\
}while(0)

chatServer::chatServer(const char* ip,int port,int threadNums):tPool(threadNums),running(true){
    //创建服务器用于通信的套接字
    this->sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd < 0){
        ERR_LOG("socket error");
        return;
    }

    //设置ip地址和端口号快速重用
    int reuse = 1;
    if(setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse)) < 0){
        ERR_LOG("setsockpot error");
        return;
    }
    if(setsockopt(sfd,SOL_SOCKET,SO_REUSEPORT,&reuse,sizeof(reuse)) < 0){
        ERR_LOG("setsockpot error");
        return;
    }

    //创建地址信息结构，将套接字和其绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);
    if(bind(sfd,(struct sockaddr*)&saddr,sizeof(saddr)) < 0){
        ERR_LOG("bind error");
        return;
    }

    //开启监听
    if(listen(sfd,10) == -1){
        ERR_LOG("listen error");
        return;
    }

}

chatServer::~chatServer()
{
    //关闭服务器套接字
    if(sfd >= 0){
        close(sfd);
    }

    //把所有用于和客户端通信的套接字进行关闭
    for(auto it = clients.begin();it != clients.end();it++){
        if(it->cfd >= 0){
            close(it->cfd);
        }
    }
}


//启动服务器
void chatServer::Run_ChatServer(){
    //循环跑服务器
    while(running){
        //定义客户端地址信息结构体
        struct sockaddr_in caddr;
        socklen_t socklen = sizeof(caddr);
        //接收客户端连接
        int cfd = accept(sfd,(struct sockaddr*)&caddr,&socklen);
#if 1
        cout << inet_ntoa(caddr.sin_addr) << ":" << ntohs(caddr.sin_port) <<" 连接成功" << endl;
#endif
        if(cfd < 0){
            ERR_LOG("accept error");
            continue;
        }

        //将和客户端通信的任务加入任务容器中
        tPool.Add_Task([this,cfd,caddr]{
            this->Handle_ClientMsg(cfd,caddr);
        });
    }
}


//处理客户端发来的信息
void chatServer::Handle_ClientMsg(int cfd,const struct sockaddr_in& caddr){
    //定义消息变量解析消息
    MSG msg;
    //定义缓冲区
    char buf[sizeof(MSG)] = "";
    //循环接收客户端的消息
    while(true){
        //清空缓冲区
        memset(buf,0,sizeof(buf));

        //接收客户端消息
        int res = recv(cfd,buf,sizeof(buf),0);
        if(res <= 0){
            //将退出消息广播给除当前客户端以外的其他客户端
            msg.type = htonl(QUIT);
            Broadcast(msg.Serialize(),cfd);

            //将当前客户端从客户端容器中移除
            {
                //给客户端容器加锁
                unique_lock<mutex> lock(cliLock);
                for(auto it = clients.begin();it != clients.end();it++){
                    if(it->cfd == cfd){
                        clients.erase(it);
                        break;
                    }
                }
            }   
            break;
        }
 
        //解析消息
        msg.Deserialize(string(buf,res));
        //根据类型进行不同处理
        int t = ntohl(msg.type);
#if 1
        cout << t << " " << msg.name << " " << msg.content << endl;
#endif
        if(t == LOGIN){
            //将上线消息广播给所有客户端
            Broadcast(msg.Serialize());

            //将客户端加入到容器中
            {
                //给客户端容器加锁
                unique_lock<mutex> lock(cliLock);
                clients.push_back({cfd,caddr});
            }
        }else if(t == CHAT){
            //将聊天消息广播给除当前客户端以外的其他客户端
            Broadcast(msg.Serialize(),cfd);

        }else if(t == QUIT){
            //将退出消息广播给除当前客户端以外的其他客户端
            Broadcast(msg.Serialize(),cfd);

            //将当前客户端从客户端容器中移除
            {
                //给客户端容器加锁
                unique_lock<mutex> lock(cliLock);
                for(auto it = clients.begin();it != clients.end();it++){
                    if(it->cfd == cfd){
                        clients.erase(it);
                        break;
                    }
                }
            }   
            break;
        }else{
            cout << "消息类型有误" << endl;
            continue;
        }
    }

    //确保关闭当前客户端套接字
    if(cfd >= 0){
        close(cfd);
    }
}


//广播发送消息
void chatServer::Broadcast(string msg,int excludfd){
    for(auto it = clients.begin();it != clients.end();it++){
        if(it->cfd != excludfd){
            if(send(it->cfd,msg.c_str(),msg.length(),0) < 0){
                ERR_LOG("broadcast send error");
                continue;
            }
        }
    }
}
