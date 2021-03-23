/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：FindPasswd.h
*   创 建 者：@liuchen
*   创建日期：2021年02月15日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once

#include<iostream>
#include<stdio.h>
#include<string>
#include<functional>

using namespace std;
class Mail {

    public:
        static Mail * getMailInstance();
        bool sendMessage(string str);
    private:
        ~Mail();
        Mail();
        FILE * _fp;

};
