/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：IoServer.cpp
*   创 建 者：@liuchen
*   创建日期：2021年01月30日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include<thread>
#include"IoServer.h"
#include<iostream>
#include"LoadToZk.h"
using namespace std;

IoServer * IoServer::_ioserver = nullptr;

IoServer* IoServer::getInstance() {

    if(_ioserver == nullptr) {
        static EventLoop loop;
        static InetAddress addr("0.0.0.0",6000);
        _ioserver = new IoServer(&loop,addr,"IOServer");

    }

    return _ioserver;
}

IoServer::IoServer(EventLoop *loop,
        const InetAddress &addr,
        const string &name):_server(loop,addr,name),_loop(loop) {
    //绑定连接回调
    _server.setConnectionCallback(bind(&IoServer::onConnection,this,_1));
    //绑定读写回调
    _server.setMessageCallback(bind(&IoServer::onMessage,this,_1,_2,_3));
    //设置线程数量 I/O 线程 和工作线程
    _server.setThreadNum(4);

    // 从db 加载停车位信息到zk 中
   LoadToZK loadToZk;  
   loadToZk.start();
    
    // 登录
    function<string(string)> func_login = LoginFunc;
    _funcMap.insert({"Login",func_login});

    // 找回密码 
    function<string(string)> func_findpwd = FindPwdFunc;
    _funcMap.insert({"FindPassWd",func_findpwd});
    // 注册
    function<string(string)> func_register = RegisterFunc;
    _funcMap.insert({"Register",func_register});

    // 预定车位
    function<string(string)> func_selectCar = SelectCar;
    _funcMap.insert({"SelectCar",func_selectCar});

}

void IoServer::start() {

    _server.start();
    _loop->loop();
}


void IoServer::onConnection(const TcpConnectionPtr &conn) {
    
    if(!conn->connected()) {
        conn->shutdown();
    }
}

string IoServer::splitMessage(string msg) {
    
    int header_size = 0;

    msg.copy((char*)&header_size, sizeof(int), 0); 
    // 本服务器为小端，需将特定信息转成网络字节序---> 大端
    header_size = htonl(header_size);
    // 消息头
    cout<<header_size<<endl;
    string header_string = msg.substr(4, header_size); 
    Head head;
    head.ParseFromString(header_string);
    header_string = head.head();
    // 消息体
    string body_string = msg.substr(4 + header_size);
    //查找对应的数据处理函数
     
    cout<<"header_string is ="<<header_string<<endl;
    cout<<"_funcMap size is = "<<_funcMap.size()<<endl;
 
    auto func = _funcMap.find(header_string); 

    string err_info = "没有这个服务";
    if(func != _funcMap.end()) {

        cout<<"找到login方法"<<endl;
        return func->second(body_string);
    }

    /*
     function<string(string)> func_login = LoginFunc;
     return func_login(body_string);
*/
    return err_info;
}

void IoServer::onMessage(const TcpConnectionPtr &conn, Buffer *buffer, Timestamp time) {

    string message = buffer->retrieveAllAsString();


    if(message == "Client_Heart") {

        cout<<message<<endl;
        //conn->send("ok");
    }else {

        conn->send(splitMessage(message));
        
   }
}

string IoServer::LoginFunc(string msg) {

    string response;
    Login login;
    response = login.verifyLogin(msg);
    return response;

}

string IoServer::FindPwdFunc(string msg) {
    string response;
    Findpwd findpwd;
    response = findpwd.findUserPwd(msg);
    return response;
}

string IoServer::RegisterFunc(string msg) {

    string response;
    Register _register;
    response = _register.registerToDatabase(msg);
    return response;
}

string IoServer::SelectCar(string msg) {

    string response;

    return response;

}
