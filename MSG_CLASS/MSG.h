#ifndef MSG_H
#define MSG_H

#include <string>
#include <string.h>

using namespace std;

#define NAMESIZE 20
#define MSGSIZE 512

//定义消息类型
#define LOGIN 1
#define CHAT 2
#define QUIT 3

class MSG
{
public:
    int type; //消息的类型
    char name[NAMESIZE]; //发消息的用户的用户名
    char content[MSGSIZE]; //消息正文

    //将消息序列化为字符串形式，以便统一形式传输
    string Serialize();

    //将字符串形式的消息反序列化
    void Deserialize(const string& msg);
};


#endif