/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：CommonConnectionPool.h
*   创 建 者：@liuchen
*   创建日期：2021年01月31日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once

#include<string>
#include<iostream>
#include<queue>
#include<mutex>
#include<atomic>
#include<thread>
#include<memory>
#include<functional>
#include<condition_variable> //条件变量
#include"Connection.h"
using namespace std;

class ConnectionPool {

    public:
        // 获取唯一的实例
       static ConnectionPool * getConnectionPool();
       // 提供从连接池中获取连接
       shared_ptr<Connection>  getConnection();
    private:
        // 单例
        ConnectionPool();
        // 从配置文件加载数据
        bool loadConfigFile();
        // 运行在独立的线程中，负责生产新连接
        void produceConnectionTask();
        // 扫描超过maxIdleTime 回收
        void scannerConnectionTask();
        string  _ip;             //  ip 地址
        unsigned short _port;    // 端口
        string _username;        // 用户名
        string _password;        // 密码
        string _dbname;         // 数据库名称
        int _initSize;           // 初始连接池连接量
        int _maxSize;            // 连接池最大连接量
        int _maxIdleTime;        // 连接池最大空闲时间
        int _connectionTimeout;  // 连接池获取连接的超时时间

        queue<Connection *> _connectionQue; // 存储musql 连接队列
        mutex _queueMutex;      // 维护队列的互斥锁
        atomic_int _connectionCnt; //记录连接所创建的connection的连接总数量
        condition_variable _cv;     // 条件变量 ，生产者与消费者通信


};
