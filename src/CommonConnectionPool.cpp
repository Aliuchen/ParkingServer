/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：CommonConnectionPool.cpp
*   创 建 者：@liuchen
*   创建日期：2021年02月01日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include<stdlib.h>
#include"CommonConnectionPool.h"
#include"public.h"
using namespace std;

// 线程安全的懒汉单例模式
ConnectionPool* ConnectionPool::getConnectionPool() {

    static ConnectionPool pool;
    return &pool;
}

ConnectionPool::ConnectionPool() {

    if(!loadConfigFile()) {

        return;
    }

    for(int i = 0 ; i < _initSize ; i++ ) {

        
        Connection * p = new Connection();
        if(!p->connect(_ip,_port,_username,_password,_dbname)) {
            return;
        }
        p->refreshAliveTime();
        _connectionQue.push(p);
        _connectionCnt++;
    }

    // 启动一个新的线程 作为生产者线程
    thread produce(std::bind(&ConnectionPool::produceConnectionTask,this));
   produce.detach();
    // 启动一个线程 ，扫描超过maxIdleTime 回收
   thread scanner(std::bind(&ConnectionPool::scannerConnectionTask,this));
   scanner.detach();
}
// 加载配置项
bool ConnectionPool::loadConfigFile() {

    FILE *fp = fopen("mysql.cnf","r");
    if(nullptr == fp) {

        LOG("mysql.cnf is not exist!" );
        return false;
    }

    while(!feof(fp)) {
        
        char line[1024] = {0};
        fgets(line,1024,fp);
        string str = line;
        int idx = str.find('=',0);
        if(idx == -1) {

            continue;
        } 
        int endidx = str.find('\n',idx);
        string key = str.substr(0,idx);
        string value = str.substr(idx+1,endidx-idx-1);

        if(key == "ip") {
            
            _ip = value;

        }else if(key == "port") {

            _port = atoi(value.c_str());

        }else if(key == "username") {

            _username = value;

        }else if(key == "password") {

            _password = value;


        }else if(key == "dbname") {

            _dbname = value;

        }else if(key == "initSize") {

            _initSize = atoi(value.c_str());

        }else if(key == "maxSize") {

            _maxSize = atoi(value.c_str());

        }else if(key == "maxIdleTime") {

            _maxIdleTime = atoi(value.c_str());

        }else if(key == "connectionTimeOut") {

            _connectionTimeout = atoi(value.c_str());
        }

    }

    if(_ip.empty() || _port== 0 || _username.empty() || _password.empty() || _dbname.empty()                                                              || _initSize == 0 || _maxSize == 0 || _maxIdleTime == 0 || _connectionTimeout == 0){
 
        LOG("Incomplete configuration information！");
             return false;

         }

    return true;
}
//  独立线程，负责生产新的连接
void ConnectionPool::produceConnectionTask() {

    for(;;) {

        unique_lock<mutex> lock(_queueMutex);

        while(!_connectionQue.empty()) {

            _cv.wait(lock); //  队列不为空等待
        }

        if(_connectionCnt < _maxSize) {
            Connection * p = new Connection();
            if(!p->connect(_ip,_port,_username,_password,_dbname)) {

               return;
            }
            p->refreshAliveTime();
            _connectionQue.push(p);
            _connectionCnt++;
        }
        _cv.notify_all();
    }
}


// 从连接池获取一个空闲连接
shared_ptr<Connection> ConnectionPool::getConnection() {

     unique_lock<mutex> lock(_queueMutex);
     while(_connectionQue.empty()) {

         if(cv_status::timeout == _cv.wait_for(lock,chrono::milliseconds(_connectionTimeout))) {
           //  如果是超时出来的 退出。如果是被唤醒，再次检查一下队列是否为空 
            if(_connectionQue.empty()) {

             LOG("获取空闲连接超时.....获取失败 !");
             return nullptr;

            }
        }
     }
     
     //  自定义删除器 ，当使用完后归还给队列
     shared_ptr<Connection> sp(_connectionQue.front(),[&](Connection *pcon) {
             unique_lock<mutex> lock(_queueMutex);
             _connectionQue.push(pcon);
             pcon->refreshAliveTime();
             });
     _connectionQue.pop();

     _cv.notify_all();    //  通知生产者 是否消费完

     return sp;

}
// 扫描超过maxIdleTime 回收函数
 
void ConnectionPool::scannerConnectionTask() {

    for(;;) {

        this_thread::sleep_for(chrono::seconds(_maxIdleTime));
        // 扫面整个队列，释放多余连接
        unique_lock<mutex> lock(_queueMutex);
        while (_connectionCnt > _initSize) {

            Connection *p = _connectionQue.front(); 

            if(p->getAliveTime() >= (_maxIdleTime*1000)) {

                _connectionQue.pop();
                _connectionCnt--;
                delete p;
            }else {
                // 如果队头都没有超过最大时间 ，则没有连接超过最大时间
                break;
            }
        }
    }
}
