/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：ZkClient.cpp
*   创 建 者：@liuchen
*   创建日期：2021年03月20日
*   描    述：
*
*   更新日志：
*
* 
================================================================*/

#include<thread>
#include<chrono>
#include"ZkClient.h"

using namespace std;

sem_t ZKClient::_sem;
string ZKClient::_rootNodePath = "/ParkingService";

ZKClient::ZKClient() :_zhandle(nullptr) {

    sem_init(&_sem, 0, 0);
}

void ZKClient::start() {

     // zookeeper_init连接zkServer创建好session是一个异步的过程
       const char *host = "127.0.0.1:2181";
       _zhandle = zookeeper_init(host,global_watcher,30000,nullptr,nullptr,0);
       

       if(_zhandle == nullptr) {

           cout << "connecting zookeeper error..." << endl;
           exit(EXIT_FAILURE);
       }

       // 阻塞等待连接成功，在返回
       sem_wait(&_sem);
        cout << "connecting zookeeper success..." << endl;
    
        
}


void ZKClient::start(watcher_fn fn) {

     // zookeeper_init连接zkServer创建好session是一个异步的过程
       const char *host = "127.0.0.1:2181";
     _zhandle = zookeeper_init(host,fn,30000,nullptr,nullptr,0);
   

       if(_zhandle == nullptr) {

          cout << "connecting zookeeper error..." << endl;
           exit(EXIT_FAILURE);
       }

      // 阻塞等待连接成功，在返回
       sem_wait(&_sem);
        cout << "connecting zookeeper success..." << endl;
    
 }

// zk创建节点
void ZKClient::create(const char *path, const char *data, int datalen, int state) {

    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
     // 判断znode节点是否存在，不能在创建
     int flag;
    flag = zoo_exists(_zhandle, path, 0, nullptr);
     if (ZNONODE == flag)                                                                                                                               
       {
        // 表示znode节点不存在，创建临时节点
         flag = zoo_create(_zhandle, path, data, datalen,
             &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
         if (flag == ZOK)
         {
            cout << "znode create success... path:" << path << endl;
        }
         else
         {
            cout << "flag:" << flag << endl;
             cout << "znode create error... path:" << path << endl;
            exit(EXIT_FAILURE);
        }
    }

}

// get znode节点的值
 string ZKClient::getVal(const char *path,int state) {

     char buffer[64];
     int bufferlen = sizeof(buffer);
     int flag = zoo_get(_zhandle, path, state, buffer, &bufferlen, nullptr);
     if (flag != ZOK) {
      cout << "get znode error... path:" << path << endl;
         return nullptr;
    } else{
        
        string val = buffer;
        val = val.at(0);
         return val;
    }
 }
              

// set node 节点的值
bool ZKClient::setVal(const char *path,const char* data, int datalen) {

    int flag = zoo_set(_zhandle,path,data,datalen,-1);

    if(flag != ZOK) {

        cout<<"set nodeVal error ...path is" <<path<<endl;
        return false;

    } else {

        return  true;
    }
}

 void ZKClient::sendHeartBeat()
 {
     // 启动线程发送zk心跳，维护session
     thread t([&]() {
         for (;;)
         {
            // 心跳时间设置为timeout时间的1/3
            int time = 30000*1.0/3;
            this_thread::sleep_for(chrono::seconds(time));
            zoo_exists(_zhandle, _rootNodePath.c_str(), 0, nullptr);
        }
   });
    t.detach();
 }
 
 // 设置存放RPC服务的根节点名称
 void ZKClient::setRootPath(string path) {
     _rootNodePath = path;
 }
 
 string ZKClient::getRootPath() {
     return _rootNodePath;
 }


 // 这个watcher是全局的回调
 void ZKClient::global_watcher(zhandle_t *zh, int type,
     int state, const char *path, void *watcherCtx) {
     cout << "watcher type:" << type << endl;
     cout << "watcher state:" << state << endl;
     cout << "watcher path:" << path << endl;
 
     // session有关的事件
    if (type == ZOO_SESSION_EVENT) {
        // session创建成功了
        if (state == ZOO_CONNECTED_STATE) {
            // 通知调用线程连接成功
            sem_post(&_sem); 
         } else if (state == ZOO_EXPIRED_SESSION_STATE) {
             // session超时了    
 

     }
    }
    if(type == ZOO_CHANGED_EVENT) {


        string tmp = path;
        cout<<path<<endl;
    
        
    }
 }              

vector<string> ZKClient::getNodeChildren(const char* path) {

    struct String_vector vec;
    vector<string> vec_path;

    int flag = zoo_get_children(_zhandle, path,0,&vec);

    if(flag != ZOK) {

        cout<<"获取子节点失败  path is" <<path<<endl;

    }else {

        if(vec.data) {

            for(int i = 0 ;i < vec.count; i++) {

                vec_path.push_back(vec.data[i]);

            }

        }
    }

    return vec_path;

}
