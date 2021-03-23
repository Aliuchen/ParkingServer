/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：SelectCar.h
*   创 建 者：@liuchen
*   创建日期：2021年03月23日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once
#include<string>
#include<stdio.h>
#include<iostream>
#include"ZkClient.h"
#include"msg_SelectCar.pb.h"
#include<arpa/inet.h>

class SelelctCar 
{
    public:
        SelelctCar();
        string getNodeVal(string msg);
    private:
        ZKClient _zk;
    
};
