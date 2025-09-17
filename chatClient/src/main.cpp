#include "chatClient.h"


int main(int argc, const char *argv[])
{
    //判断参数是否上传
    if(argc < 4){
        cout << "请输入服务器ip地址、端口号和用户名" << endl;
    }

    try
    {
        //实例化客户端对象
        chatClient chatCli(argv[1],atoi(argv[2]),argv[3]);

        //运行客户端
        chatCli.Run_ChatClient();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    return 0;
}
