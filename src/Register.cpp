/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Register.cpp
*   创 建 者：@liuchen
*   创建日期：2021年02月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"Register.h"
using namespace std;

Register::Register() {

    _pool = ConnectionPool::getConnectionPool();
    _connection = _pool->getConnection();
}

string Register::registerToDatabase(string msg) {

    
    string res_head = "registerRes";
    string res_body;
    string res_head_info;
    string response_send_str;

    RegisterRequest registerRequest;
    registerRequest.ParseFromString(msg);

    string uname = registerRequest.username();
    string upasswd = registerRequest.password();
    string umail = registerRequest.usermail();
    
    RegisterResponse registerResponse;

    string sql_select = "select user_name from userInfo where user_name = '"+uname+"'";
    MYSQL_RES * res = _connection->query(sql_select);
    MYSQL_ROW row;
    if(res == nullptr) {

        registerResponse.set_code(4);
        registerResponse.set_info("服务器错误");
    }else {

        row = mysql_fetch_row(res);
        if(row != nullptr) {

            registerResponse.set_code(2);
            registerResponse.set_info("该用户已经注册");
        }else {

             string sql_info = "insert into userInfo(user_name,user_pwd,user_mail) values('"+uname+"','"+upasswd+"','"+umail+"')";
             if(_connection->updata(sql_info)) {
                 registerResponse.set_code(1);
                 registerResponse.set_info("注册成功");
             }else {

                 registerResponse.set_code(3);
                 registerResponse.set_info("注册失败");
             }
        }

    }
    registerResponse.SerializeToString(&res_body);
     Head head;
     head.set_head(res_head);
     head.SerializeToString(&res_head_info); 
     int head_size = htonl(res_head_info.size()); 
     response_send_str.insert(0, string((char*)&head_size, 4));

    return response_send_str+res_head_info+res_body;
}
