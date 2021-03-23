/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Connection.h
*   创 建 者：@liuchen
*   创建日期：2021年01月31日
*   描    述：Mysql 操作
*
*   更新日志：
*
================================================================*/
#pragma once 
#include<mysql/mysql.h>
#include<string>
#include<ctime>
using  namespace std;
class Connection {

    public:
        // 初始化数据库连接
        Connection();
        // 释放数据库连接资源
        ~Connection();
        // 刷新连接起始空闲时间点
        void refreshAliveTime();
        // 返回存活的时间
        clock_t getAliveTime()const ;
        //连接数据库
        bool connect (string ip, 
                unsigned short port, 
                string user, 
                string password, 
                string dbname);

        // 更新操作 insert delete updata 
        bool updata(string sql);

        // 查询操作
        MYSQL_RES * query(string sql);

    private:
        MYSQL * _conn;
        clock_t _alivetime;  //  记录空闲状态的时间

};
