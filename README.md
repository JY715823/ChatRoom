# 🎯 ChatRoom - C++ TCP Network Chat Room

## 🌓 项目简介
基于TCP协议的高并发网络聊天室，使用C++11实现，支持多客户端同时在线聊天。

## 🧩 技术技术
- C++11
- Socket网络编程
- TCP协议
- 线程池设计模式
- 多线程编程
- 条件化架构

## 🔧 功能特性
✅ 支持多客户端并发连接  
✅ 使用线程池处理高并发请求  
✅ 支持登录、聊天、退出三种消息类型  
✅ 实时广播消息给所有在线用户  
✅ 客全服务器端口重用（SO_REUSEADDR）  

## 🧩 编译与运行

### 服务器端
```bash
cd chatServer
mkdir build && cd build
cmake ..
make
./chatServer 127.0.0.1 8888