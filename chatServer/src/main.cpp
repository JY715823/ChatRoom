
#include "chatServer.h"

int main(int argc, const char *argv[])
{
    //检查参数是否正确
    if(argc < 3){
        cout << "请输入ip地址和端口号！" << endl;
        return -1;
    }

    try
    {
        //实例化聊天室服务端对象
        chatServer chatSer(argv[1],atoi(argv[2]),THREADNUM);

        //启动服务器
        chatSer.Run_ChatServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }    

    return 0;
}