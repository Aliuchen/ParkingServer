/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：main.cpp
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
#include<ctime>
#include"Connection.h"
#include"CommonConnectionPool.h"
using namespace std;

int main()
{
    clock_t begin = clock();
    ConnectionPool *p = ConnectionPool::getConnectionPool(); 
    for(int i = 0; i < 1000; i++) {
 /*       
    Connection conn;
    char sql[1024] ={0};
    sprintf(sql,"insert into login(tel,password) values('%s', '%s')","15591485221","LC199938");
    conn.connect("127.0.0.1",3306,"root", "LC199938", "parking_service");
    conn.updata(sql);
   */  
    
        shared_ptr<Connection> sp = p->getConnection();
     char sql[1024] ={0};
      sprintf(sql,"insert into login(tel,password) values('%s', '%s')","15591485221","LC199938");  
    sp->updata(sql);
   
    }
    clock_t end = clock();
    
    cout<<end-begin<<endl;
    return 0;
}
