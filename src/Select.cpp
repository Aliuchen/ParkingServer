/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：Select.cpp
*   创 建 者：@liuchen
*   创建日期：2021年03月29日
*   描    述：
*
*   更新日志：
*
================================================================*/

#include <arpa/inet.h>
#include"Select.h"

using namespace std;

Select::Select() {

    _pool = ConnectionPool::getConnectionPool();
    _connection = _pool->getConnection();

}

string Select::getUserInfo(string msg) {


     string res_head = "LoadUserInfoRes";
     string res_body;
     string res_head_info;
     string response_send_str;


     PersonInfoRequest infoRequest;
     infoRequest.ParseFromString(msg);


     string userTel = infoRequest.telnum();

     PersonInfoResponse infoResponse;

     string sql_info = "select person_name, person_id,user_mail,car_name,car_number,member,balance from userInfo where user_name = '"+userTel+"'";
      
     MYSQL_ROW row;
     MYSQL_RES *res = _connection->query(sql_info);


     if(res == nullptr) {

         infoResponse.set_code(2);

     } else {


         row = mysql_fetch_row(res);

         if(row != nullptr)  {

             infoResponse.set_code(1);
             
             if(row[0] != nullptr) {

                 infoResponse.set_personname(row[0]);

             }

             if(row[1] != nullptr) {

                 infoResponse.set_userid(row[1]);
                
             }

             if(row[2] != nullptr) {

                 infoResponse.set_usermail(row[2]);
             }

             if(row[3] != nullptr) {

                 infoResponse.set_carname(row[3]);
             }

             if(row[4] != nullptr) {

                 infoResponse.set_carid(row[4]);

             }

             if(row[5] != nullptr) {

                 infoResponse.set_member(row[5]);

             }

             if(row[6] != nullptr) {

                 infoResponse.set_balance(row[6]);
             }else {

                 infoResponse.set_balance("0.00");

             }

         } 

     }
          infoResponse.SerializeToString(&res_body);;
          Head head;
          head.set_head(res_head);
          head.SerializeToString(&res_head_info);
          int head_size = htonl(res_head_info.size());  // 转成大端
          response_send_str.insert(0, string((char*)&head_size, 4));

           return response_send_str+res_head_info+res_body;


}

string Select::rechargeMoney(string msg) {

     string res_head = "payMoneyRes";

    string res_body;
    string res_head_info;
    string response_send_str;

    RechargeRequest rechargeRequest;
    rechargeRequest.ParseFromString(msg);

    string pwd = rechargeRequest.paypwd();
    string tel = rechargeRequest.usertelnum();
    double money = rechargeRequest.money();



    RechargeResponse rechargeResponse;
    string sql_info = "select pay_pwd,balance from userInfo where user_name = '"+tel+"'";


     MYSQL_ROW row;
     MYSQL_RES *res = _connection->query(sql_info);

     if(res == nullptr) {

         rechargeResponse.set_code(4);
         rechargeResponse.set_info("服务器错误");
         rechargeResponse.set_balance("0.00");
     }else {


         row = mysql_fetch_row(res);

         if(row != nullptr) {


             if(row[0] == nullptr) {

              rechargeResponse.set_code(3);
              rechargeResponse.set_info("还未设置支付密码");
              rechargeResponse.set_balance("0.00");
             } else  if(row[0] != nullptr &&row[0] != pwd) {

                 rechargeResponse.set_code(2);
                 rechargeResponse.set_info("支付密码错误");
                 rechargeResponse.set_balance("0.00");
             } else {

                 string val;
                 if(row[1] != nullptr) {

                     val = row[1];

                     double begin = atof(val.c_str());
                     money = begin + money;

                 }

                 shared_ptr<Connection> con = _pool->getConnection();
                 string sql_update = "update userInfo set balance="+to_string(money)+" where user_name = '"+tel+"'";
                 if(con->updata(sql_update)) {

                     cout<<"更新成功了"<<endl;
                     rechargeResponse.set_code(1);
                     rechargeResponse.set_info("充值成功");
                     string tmp = to_string(money);
                     rechargeResponse.set_balance(tmp);
                 } else {

                     rechargeResponse.set_code(3);
                     rechargeResponse.set_info("充值失败");
                     rechargeResponse.set_balance("0.00");
                 }


             }

         } 

    }

     rechargeResponse.SerializeToString(&res_body);
     Head head;
    head.set_head(res_head);
    head.SerializeToString(&res_head_info);
    int head_size = htonl(res_head_info.size());  // 转成大端
    response_send_str.insert(0, string((char*)&head_size, 4));

    return response_send_str+res_head_info+res_body;
}
string Select::isSetPayPwd(string msg) {

    string res_head = "IsSetPayPwdRes";
    string res_body;
    string res_head_info;
    string response_send_str;

    IsSetPayPwdRequest  request ;
    request.ParseFromString(msg);


    string userTel = request.usertel();
    IsSetPayPwdResponse response;

    if(userTel.size() != 0) {

        string sql_info = "select pay_pwd from userInfo where user_name = '"+userTel+"'";
        MYSQL_RES *res = _connection->query(sql_info);
        MYSQL_ROW row = mysql_fetch_row(res);

        if(row != nullptr && row[0] !=nullptr) {
            response.set_code(1);
        } else {
            response.set_code(0);

        }
    } else {
        response.set_code(0);

    } 

    response.SerializeToString(&res_body);
     Head head;
    head.set_head(res_head);
    head.SerializeToString(&res_head_info);
    int head_size = htonl(res_head_info.size());  // 转成大端
    response_send_str.insert(0, string((char*)&head_size, 4));

    return response_send_str+res_head_info+res_body;

}

string Select::orderDispose(string msg) {

    string res_head = "submitOrderRes";
    string res_body;
    string res_head_info;
    string response_send_str;

    OrderSubmitRequest request;
    request.ParseFromString(msg);

    string userTel = request.usertel();
    string paypwd = request.pwd();
    double consume = request.consume();

    OrderSubmitResponse response;

    if(userTel.size() != 0) {

        string sql_info = "select pay_pwd,balance from userInfo where user_name = '"+userTel+"'";
        MYSQL_RES *res = _connection->query(sql_info);
        MYSQL_ROW row = mysql_fetch_row(res);

        if(row != nullptr && row[0] != nullptr) {
            if(row[0] != paypwd) {
                response.set_code(2);
                if(row[1] != nullptr) {
                    response.set_money(row[1]);
                }else {
                    response.set_money("0.00");
                }
                response.set_info("支付密码错误");
            }else {
                if(row[1] != nullptr) {
                    string tmp = row[1];
                    double val = atof(tmp.c_str());

                    if(consume > val) {
                        response.set_code(3);
                        response.set_money(row[1]);
                        response.set_info("余额不足");
                    }else {
                        double money = val - consume;
                        string orderId = request.orderid();
                        string location = request.location();
                        string startReserveTime = request.startreservetime();
                        string startParkTime = request.startparkingtime();
                        string endTime = request.endtime();
                        int32_t parkTag = request.parktag();

                        string sql_order = "insert into orderInfo(userName,orderId,location,startReserveTime,startParkTime,endTime,parkTag,payTag,consume) values('"+userTel+"','"+orderId+"','"+location+"','"+startReserveTime+"','"+startParkTime+"','"+endTime+"','"+to_string(parkTag)+"','1','"+to_string(consume)+"')";
                        shared_ptr<Connection> con1 = _pool->getConnection();

                        if(con1->updata(sql_order)) {

                        string sql_update = "update userInfo set balance="+to_string(money)+" where user_name = '"+userTel+"'";

                        shared_ptr<Connection> con2 = _pool->getConnection();

                        if(con2->updata(sql_update)) {

                        response.set_code(1);
                        response.set_money(to_string(money));
                        response.set_info("订单已经支付");
                        }
                        }
                    }
                }else {
                    response.set_code(3);
                    response.set_money("0.00");
                    response.set_info("余额不足");
                }
            }
        }

    }else {

        response.set_code(4);
        response.set_money("0.00");
        response.set_info("用户名错误");

    }

    response.SerializeToString(&res_body);

    Head head;
    head.set_head(res_head);
    head.SerializeToString(&res_head_info);
    int head_size = htonl(res_head_info.size());  // 转成大端
    response_send_str.insert(0, string((char*)&head_size, 4));
 
    return response_send_str+res_head_info+res_body;
}
