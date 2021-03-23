/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Register.h
*   创 建 者：@liuchen
*   创建日期：2021年02月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<arpa/inet.h>
#include"CommonConnectionPool.h"
#include"Connection.h"
#include"msg_Register.pb.h"
#include"msg_Head.pb.h"


class Register {

    public:
        Register();
        string registerToDatabase(string msg);
    private:
       ConnectionPool * _pool;
       shared_ptr<Connection> _connection;
};
