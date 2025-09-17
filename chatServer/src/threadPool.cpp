#include "threadPool.h"


threadPool::threadPool(int threadNums):start(true)
{
    //启动线程池
    Start_ThreadPool(threadNums);
}

threadPool::~threadPool()
{
    //加锁，将线程池运行标志设置为关闭
    {   
        unique_lock<mutex> lock(mtx);
        start = false;
    }
    //唤醒所有工作线程
    task_cv.notify_all();

    //等待所有工作线程退出
    for(int i = 0;i < workers.size();i++){
        if(workers[i].joinable()){
            workers[i].join();
        }
    }
}

void threadPool::Start_ThreadPool(int threadNums){
    //创建对应数量的线程
    for(int i = 0;i < threadNums;i++){
        workers.emplace_back([this]{
            while(true){
                function<void()> task;//当前线程要执行的任务
                {
                    //给任务队列加锁，以便让任务安全地分配给线程
                    unique_lock<mutex> lock(mtx);

                    //等待条件变量唤醒线程，将任务分配给线程
                    task_cv.wait(lock,[this]{
                        return !start || !tasks.empty(); //唤醒条件：当线程池停止或者任务队列中有任务
                    });

                    if(!start){
                        return;
                    }

                    //将任务容器中的队头任务取出来
                    task = move(tasks.front());
                    //将任务出队
                    tasks.pop();
                }//释放锁
                
                //执行任务
                task();
            }
        });
    }
}

void threadPool::Add_Task(function<void()> task){
    //为任务容器加锁
    {
        unique_lock<mutex> lock(mtx);
        //将任务添加到容器中
        tasks.push(task);
    }//出作用域后自动释放锁

    //唤醒一个线程去工作
    task_cv.notify_one();
}