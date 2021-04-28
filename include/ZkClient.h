/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：ZkClient.h
*   创 建 者：@liuchen
*   创建日期：2021年03月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once

#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<semaphore.h>
#include<zookeeper/zookeeper.h>
using namespace std;

class ZKClient {

    public:
        ZKClient();

        void start();
        void start(watcher_fn fn);
        // zk创建节点
        void create(const char *path, const char *data, int datalen, int state=0);

         // get znode节点的值
         string getVal(const char *path,int state);
         // 发送心跳
         void sendHeartBeat();

         // set node节点的值
         bool setVal(const char* path,const char *data, int datalen);

         // 返回子节点
         vector<string> getNodeChildren(const char *path);

         static void setRootPath(string path);

         static string getRootPath();

         // 这个watcher是全局的回调
          static void global_watcher(zhandle_t *zh, int type,
                   int state, const char *path, void *watcherCtx);    
          // 同步session创建成功
          static sem_t _sem;
    private:
         // zkclient和zkserver通信用的句柄
     zhandle_t *_zhandle;
     // 存放根结点
     static string _rootNodePath;

};
