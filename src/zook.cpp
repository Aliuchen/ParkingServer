/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：zook.cpp
*   创 建 者：@liuchen
*   创建日期：2021年03月20日
*   描    述：
*
*   更新日志：
*
================================================================*/

#include"ZkClient.h"
int main()
{
    ZKClient c;

    c.start();
    c.sendHeartBeat();
    
    string s = "经度34.22987464828418纬度108.95468917825438";

    string rootNode = c.getRootPath();


    c.create(rootNode.c_str(),nullptr,-1);

    return 0;
}
