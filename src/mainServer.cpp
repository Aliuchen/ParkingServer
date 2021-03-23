/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：mainServer.cpp
*   创 建 者：@liuchen
*   创建日期：2021年02月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include"IoServer.h"
using namespace std;

int main()
{
    IoServer * server = IoServer::getInstance();
    server->start();
    return 0;
}
