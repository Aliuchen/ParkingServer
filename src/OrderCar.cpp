/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：OrderCar.cpp
*   创 建 者：@liuchen
*   创建日期：2021年03月25日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"OrderCar.h"
using namespace std;

OrderCar::OrderCar(ZKClient * zk) {

    _zk = zk;
    
}

string OrderCar::orderCarASetVal(string msg) {


    OrderRequest orderRequest;
    orderRequest.ParseFromString(msg);

    string longitude = orderRequest.longitude();
    string latitude = orderRequest.latitude();
    string area = orderRequest.area();
    string location = orderRequest.location();

    vector<string> vec;
    vec.push_back(area);
    vec.push_back(location);

    string path = _zk->getRootPath();

    path = path+ "/经度"+longitude+"纬度"+latitude +"/"+area+"/"+location;


    string val = _zk->getVal(path.c_str(),1);

    string sendVal;
    if(val == "1") {


        vec.push_back("该位置已经被其他用户预定");
        sendVal = sendMessage(2,vec);

    }else {

        vec.push_back("预定成功");


       
        val = _zk->getVal(path.c_str(),1);

        if(val == "0" && _zk->setVal(path.c_str(),to_string(1).c_str(),to_string(1).size())) {


        sendVal = sendMessage(1,vec);
        }else {

            vec.push_back("请重新尝试");

            sendVal  = sendMessage(3,vec);

        }


    }

    return sendVal;
}

string OrderCar::sendMessage(int code,vector<string> &vec) {

    string res_head ="OrderCarEndRes";
    string res_body;

    string res_head_info;
    string response_send_str;

    OrderResponse orderResponse;


    orderResponse.set_code(code);
    orderResponse.set_area(vec[0]);
    orderResponse.set_location(vec[1]);
    orderResponse.set_info(vec[2]);


    orderResponse.SerializeToString(&res_body);

    Head head;
    head.set_head(res_head);
    head.SerializeToString(&res_head_info);
    int head_size = htonl(res_head_info.size());  // 转成大端
    response_send_str.insert(0, string((char*)&head_size, 4));

    return response_send_str+res_head_info+res_body;

}
