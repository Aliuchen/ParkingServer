/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Login.h
*   创 建 者：@liuchen
*   创建日期：2021年02月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once

#include<stdio.h>
#include<iostream>
#include<string>
#include<stdlib.h>
#include"CommonConnectionPool.h"
#include"Connection.h"
#include"msg_Login.pb.h"
#include"msg_Head.pb.h"
class Login {

    public:
        Login();
        string verifyLogin(string msg);
    private:
        ConnectionPool * _pool;
        shared_ptr<Connection> _connection;

};
