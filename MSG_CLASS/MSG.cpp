#include "MSG.h"

string MSG::Serialize(){
    string msg;
    msg.append(reinterpret_cast<const char*>(&type),sizeof(type)); //将类型强转为const char*形式加在msg后
    msg.append(name,sizeof(name));
    msg.append(content,sizeof(content));

    return msg;
}

void MSG::Deserialize(const string& msg){
    //定义偏移量
    size_t offset = 0;
    memcpy(&type,msg.c_str() + offset,sizeof(type)); // 将类型从消息中解析赋值
    offset += sizeof(type);

    memcpy(name,msg.c_str() + offset,sizeof(name));
    offset += sizeof(name);

    memcpy(content,msg.c_str() + offset,sizeof(content));
}