/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：LoadToZk.h
*   创 建 者：@liuchen
*   创建日期：2021年03月21日
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
#include<json/json.h>
#include"Connection.h"
#include"CommonConnectionPool.h"
#include"ZkClient.h"

class LoadToZK {

    public:
        LoadToZK();
        void start();
    private:

        ConnectionPool * _pool;
        shared_ptr<Connection> _connection;
        ZKClient _zkclient;

};
