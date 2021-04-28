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

ZKClient * IoServer::_zkClient = new ZKClient;

ConnectionPool * IoServer::_pool = ConnectionPool::getConnectionPool();

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
        const string &name):_server(loop,addr,name),_loop(loop){
    //绑定连接回调
    _server.setConnectionCallback(bind(&IoServer::onConnection,this,_1));
    //绑定读写回调
    _server.setMessageCallback(bind(&IoServer::onMessage,this,_1,_2,_3));
    //设置线程数量 I/O 线程 和工作线程
    _server.setThreadNum(4);

    // 从db 加载停车位信息到zk 中
   LoadToZK loadToZk;  
   loadToZk.start();

   _zkClient->start(global_watcher);
   _zkClient->sendHeartBeat();

    
    // 登录
    function<string(string)> func_login = LoginFunc;
    _funcMap.insert({"Login",func_login});

    // 找回密码 
    function<string(string)> func_findpwd = FindPwdFunc;
    _funcMap.insert({"FindPassWd",func_findpwd});
    // 注册
    function<string(string)> func_register = RegisterFunc;
    _funcMap.insert({"Register",func_register});

    // 预定车位初始化
    function<string(string)> func_selectCar = SelectCar;
    _funcMap.insert({"SelectCarFirst",func_selectCar});

    // 预定车位
    function<string(string)> func_orderCar = OrderCarF;
    _funcMap.insert({"OrderCarEnd",func_orderCar});

    // 个人信息设置
    function<string(string)> func_infoSet = PersonInfoSet;
    _funcMap.insert({"UserInfoSet",func_infoSet});

    // 加载用户信息
    function<string(string)> func_getUserInfo = GetUserInfo;
    _funcMap.insert({"LoadUserInfo",func_getUserInfo});

    // 充值
    function<string(string)> func_recharge = Recharge;
    _funcMap.insert({"payMoney",func_recharge});
    // 是否设置支付密码
    function<string(string)> func_isSetPayPwd = IsSetPayPwd;
    _funcMap.insert({"IsSetPayPwd",func_isSetPayPwd});

    // 订单处理
    function<string(string)> func_orderDispose = OrderDispose;
    _funcMap.insert({"submitOrder",func_orderDispose});
}

void IoServer::start() {

    _server.start();
    _loop->loop();
}


void IoServer::onConnection(const TcpConnectionPtr &conn) {
    

    if(!conn->connected()) {

        auto it = _con.find(conn);
        if(it != _con.end()){

            cout<<"移除一条连接"<<endl;
            _con.erase(it);
        }

        conn->shutdown();
    } else {

        _con.insert(conn);
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

        cout<<"找到方法"<<endl;
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
    SelelctCar car(_zkClient);
    response = car.getNodeVal(msg);

    return response;

}

string IoServer::OrderCarF(string msg) {

    string response;  
    OrderCar car(_zkClient);

    response = car.orderCarASetVal(msg);
     
    return response;

}

string IoServer::PersonInfoSet(string msg) {
    
    string response;  
    Register _register;

    response = _register.setPersonInfo(msg);
    return response;

}

string IoServer::GetUserInfo(string msg) {

    string response;

    Select _select;


    response = _select.getUserInfo(msg);

    return response;
    
}

string IoServer::Recharge(string msg) {


    string response;

    Select _select;

    response = _select.rechargeMoney(msg);

    return response;
}

string IoServer::IsSetPayPwd(string msg) {
    string response;
    Select _select;
    response = _select.isSetPayPwd(msg);
    return response;
}

string IoServer::OrderDispose(string msg) {
    string response;
    Select _select;

    response = _select.orderDispose(msg);
    return response;
}
 void IoServer::global_watcher(zhandle_t *zh, int type,
      int state, const char *path, void *watcherCtx) {
     cout << "IoServer watcher type:" << type << endl;
     cout << "IoServer watcher state:" << state << endl;
     cout << "IoServer watcher path:" << path << endl;

     // session有关的事件
    if (type == ZOO_SESSION_EVENT) {
        // sresponse;ession创建成功了
        if (state == ZOO_CONNECTED_STATE) {
            // 通知调用线程连接成功                                                                                                                                       
            sem_post(&ZKClient::_sem);
         } else if (state == ZOO_EXPIRED_SESSION_STATE) {
             // session超时了    


     }
    }
    if(type == ZOO_CHANGED_EVENT) {

        _zkClient->getVal(path,1);
        string tmp = path;
        cout<<"IoServer 的回调"<<path<<endl;

        int n = tmp.find_last_of("/");

        string head_path = tmp.substr(0,n);
        string location = tmp.substr(n+1);


        int m = head_path.find_last_not_of("/");

        string area = head_path.substr(m);

        vector<string> vec;
        vec.push_back(area);
        vec.push_back(location);
        vec.push_back("");
        IoServer * io = IoServer::getInstance();
        io->sendToEveryConn(OrderCar::sendMessage(3,vec));
 
     }

  }

void IoServer::sendToEveryConn(string msg) {


    cout<<_con.size()<<endl;
    
    for(TcpConnectionPtr ptr : _con ) {

        ptr->send(msg);
    }
}


