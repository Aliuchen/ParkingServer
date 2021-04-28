/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Register.cpp
*   创 建 者：@liuchen
*   创建日期：2021年02月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"Register.h"
using namespace std;

Register::Register() {

    _pool = ConnectionPool::getConnectionPool();
    _connection = _pool->getConnection();
}

string Register::registerToDatabase(string msg) {

    
    string res_head = "registerRes";
    string res_body;
    string res_head_info;
    string response_send_str;

    RegisterRequest registerRequest;
    registerRequest.ParseFromString(msg);

    string uname = registerRequest.username();
    string upasswd = registerRequest.password();
    string umail = registerRequest.usermail();
    string member = "VIP 1";
    RegisterResponse registerResponse;

    string sql_select = "select user_name from userInfo where user_name = '"+uname+"'";
    MYSQL_RES * res = _connection->query(sql_select);
    MYSQL_ROW row;
    if(res == nullptr) {

        registerResponse.set_code(4);
        registerResponse.set_info("服务器错误");
    }else {

        row = mysql_fetch_row(res);
        if(row != nullptr) {

            registerResponse.set_code(2);
            registerResponse.set_info("该用户已经注册");
        }else {

             string sql_info = "insert into userInfo(user_name,user_pwd,user_mail,member) values('"+uname+"','"+upasswd+"','"+umail+"','"+member+"')";
             if(_connection->updata(sql_info)) {
                 registerResponse.set_code(1);
                 registerResponse.set_info("注册成功");
             }else {

                 registerResponse.set_code(3);
                 registerResponse.set_info("注册失败");
             }
        }

    }
    registerResponse.SerializeToString(&res_body);
     Head head;
     head.set_head(res_head);
     head.SerializeToString(&res_head_info); 
     int head_size = htonl(res_head_info.size()); 
     response_send_str.insert(0, string((char*)&head_size, 4));

    return response_send_str+res_head_info+res_body;
}

string Register::setPersonInfo(string msg) {


     string res_head = "UserInfoSetRes";
     string res_body;
     string res_head_info;
     string response_send_str;

    PersonInfoSetRequest setRequest;
    setRequest.ParseFromString(msg);

    string userTel = setRequest.usertel();
    string personName = setRequest.personname();
    string personTel = setRequest.persontelnum();
    string personId = setRequest.personid();
    string carName = setRequest.carname();
    string carId = setRequest.carid();
    string payPwd = setRequest.paypwd();


    PersonInfoSetResponse setResponse;

    /*
    string sql_set = "insert into userInfo(user_name,person_name,person_id,car_name,car_id,pay_pwd) values('"+userTel+"','"+personName+"','"+personId+"','"+carName+"','"+carId+"','"+payPwd+"') on duplicate key updata person_name='"+personName+"', person_id='"+personId+"',car_name='"+carName+"', car_id"; 

*/

    string mysql_update = "update userInfo set user_name='"+personTel+"', person_name='"+personName+"',person_id='"+personId+\
                       "',car_name='"+carName+"',car_number='"+carId+"',pay_pwd='"+payPwd+"' where user_name='"+userTel+"'";

     if(_connection->updata(mysql_update)) {

        setResponse.set_code(1);
        setResponse.set_info("更新成功");

     } else {

         setResponse.set_code(2);
         setResponse.set_info("更新失败");
     }

     setResponse.SerializeToString(&res_body);
     Head head;
     head.set_head(res_head);
     head.SerializeToString(&res_head_info); 
     int head_size = htonl(res_head_info.size()); 
     response_send_str.insert(0, string((char*)&head_size, 4));

     return response_send_str+res_head_info+res_body;
}
