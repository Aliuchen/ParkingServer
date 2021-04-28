/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：OrderCar.h
*   创 建 者：@liuchen
*   创建日期：2021年03月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once

#include<stdio.h>
#include<iostream>
#include<string>
#include"ZkClient.h"
#include"msg_Order.pb.h"
#include"msg_Head.pb.h"
#include<arpa/inet.h>

class OrderCar {

    public:
        OrderCar(ZKClient * zk);
        string orderCarASetVal(string msg);
        static string sendMessage(int code,vector<string> &vec);
    private:
        ZKClient * _zk;

};
