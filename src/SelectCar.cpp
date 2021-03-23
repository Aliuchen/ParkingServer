/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：SelectCar.cpp
*   创 建 者：@liuchen
*   创建日期：2021年03月23日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"SelectCar.h"
using namespace std;

SelelctCar::SelelctCar() {
    _zk.start();
}

string SelelctCar::getNodeVal(string msg) {

    string res_head ="selectCarRes";
    string res_body;
    string res_head_info;
    string response_send_str;

    CarRequest carRequest;
    carRequest.ParseFromString(msg);

    string longitude = carRequest.longitude();
    string latitude = carRequest.latitude();


    string path = "/经度"+longitude+"纬度"+latitude;




}
