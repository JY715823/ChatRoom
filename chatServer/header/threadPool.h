#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>

//使用标准命名空间
using namespace std;

class threadPool
{
private:
    queue<function<void()>> tasks; //定义任务容器，采用队列先进后出来处理任务
    vector<thread> workers; //定义线程容器
    mutex mtx; //定义保护任务容器的互斥锁
    condition_variable task_cv; //用于唤醒线程执行任务的条件变量
    
    //启动线程池
    void Start_ThreadPool(int threadNums);

public:
    bool start; //定义线程池是否启动的标志

    threadPool(int threadNums);
    ~threadPool();

    //向任务容器中添加任务
    void Add_Task(function<void()> func); 
    

};


#endif