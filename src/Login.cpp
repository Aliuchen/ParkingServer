/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Login.cpp
*   创 建 者：@liuchen
*   创建日期：2021年02月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include "Login.h"
#include <arpa/inet.h>
using namespace std;

Login::Login() {

    _pool = ConnectionPool::getConnectionPool();
    _connection = _pool->getConnection();
}

string Login::verifyLogin(string msg) {


    string res_head = "loginRes";
    string res_body;
    string res_head_info;
    string response_send_str;

    LoginRequest loginRequest;
    loginRequest.ParseFromString(msg);

    string uname = loginRequest.username();
    string upasswd = loginRequest.password();

    LoginResponse  response; 

    string sql_info = "select user_name,user_pwd from userInfo where user_name = '"+uname+"'";

    MYSQL_ROW row;
    MYSQL_RES *res = _connection->query(sql_info);
    
    if(res == nullptr) {

        response.set_code(4);
        response.set_info("服务器错误");
    } else {

        row = mysql_fetch_row(res);

        if(row != nullptr) {

        if(upasswd == row[1]) {
            response.set_code(1);
            response.set_info("登录成功");
        }else {

            response.set_code(2);
            response.set_info("账号或密码错误");
        }

        }else {

            response.set_code(3);
            response.set_info("该用户未注册");
        }
  }
    response.SerializeToString(&res_body);
    
    Head head;
    head.set_head(res_head);
    head.SerializeToString(&res_head_info);
    int head_size = htonl(res_head_info.size());  // 转成大端
    response_send_str.insert(0, string((char*)&head_size, 4));

    return response_send_str+res_head_info+res_body;

}
