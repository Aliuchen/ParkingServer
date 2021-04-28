/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Select.h
*   创 建 者：@liuchen
*   创建日期：2021年03月29日
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
#include"msg_PersonInfo.pb.h"
#include"msg_Head.pb.h"
#include"msg_Recharge.pb.h"
#include"msg_IsSetPayPwd.pb.h"
#include"msg_OrderSubmit.pb.h"
class Select {

    public:
        Select();
        string getUserInfo(string msg);
        string rechargeMoney(string msg);
        string isSetPayPwd(string msg);
        string orderDispose(string msg);
    private:

        ConnectionPool * _pool;
        shared_ptr<Connection> _connection;


};


