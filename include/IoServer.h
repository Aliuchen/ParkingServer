/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：IoServer.h
*   创 建 者：@liuchen
*   创建日期：2021年01月30日
*   描    述：IO服务器
*
*   更新日志：
*
================================================================*/
#ifndef  IOSERVER_H
#define IOSERVER_H 

#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>
#include<muduo/base/Logging.h>
#include<functional>
#include<string>
#include<unordered_map>
#include<map>
#include<set>
#include"Login.h"
#include"msg_Head.pb.h"
#include"Findpwd.h"
#include"Register.h"
#include"SelectCar.h"
#include"OrderCar.h"
#include"Select.h"
using namespace std;
using namespace muduo::net;
using namespace muduo;
class IoServer {
    public:
        // 获取唯一的实例
        static IoServer * getInstance();
        void start();
        static ZKClient* _zkClient;
        static ConnectionPool * _pool; 
        void sendToEveryConn(string msg);
    private:
       IoServer (EventLoop *loop,const InetAddress &addr, const string &name); 
       void onConnection(const TcpConnectionPtr &conn);
       // 登录分发函数
       static string LoginFunc(string msg);
       // 找回密码分发函数
       static string FindPwdFunc(string msg);
       // 注册分发函数  
       static string RegisterFunc(string msg);
       // message 处理函数
       string splitMessage(string msg);
       // 车位预定初始化函数
       static string SelectCar(string msg);
       // 车位预定函数
       static string OrderCarF(string msg);
       // 个人信息设置
       static string PersonInfoSet(string msg);
       // 加载用户信息
       static string GetUserInfo(string msg);
       // 充值
       static string Recharge(string msg);
       // 是否设置支付密码
       static string IsSetPayPwd(string msg);
       // 订单处理  
       static string OrderDispose(string msg);
       // 这个zookeeper的回调
       static void global_watcher(zhandle_t *zh, int type,                                                                                                             
                   int state, const char *path, void *watcherCtx);    
       void onMessage(const TcpConnectionPtr &conn,
               Buffer *buffer,
               Timestamp time);
       TcpServer _server;
       EventLoop* _loop;
       static IoServer * _ioserver;
       map<string,function<string(string)>> _funcMap;
       set<TcpConnectionPtr> _con;
};

#endif

