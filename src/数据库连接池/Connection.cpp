/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Connection.cpp
*   创 建 者：@liuchen
*   创建日期：2021年01月31日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include"public.h"
#include"Connection.h"
using namespace std;

Connection::Connection() {

    _conn = mysql_init(nullptr);

}

Connection::~Connection() {

    if(_conn != nullptr) {

        mysql_close(_conn);

    }
}

void Connection::refreshAliveTime() {

    _alivetime = clock();
}

clock_t Connection::getAliveTime() const {

    return clock() - _alivetime;
}
bool Connection::connect(string ip, unsigned short port, string user, string password,
        string dbname) {

    MYSQL *p = mysql_real_connect(_conn, ip.c_str(), user.c_str(),
            password.c_str(), dbname.c_str(), 
            port, nullptr, 0);
    return p != nullptr;

}

bool Connection::updata(string sql) {

    if(mysql_query(_conn, sql.c_str())) {

       LOG("更新失败"+sql);
        return false;
    }

    return true;
}

MYSQL_RES * Connection::query(string sql) {

    if(mysql_query(_conn, sql.c_str())) {

        LOG("查询失败" + sql);
        return nullptr;
    }

    return mysql_use_result(_conn);
}
