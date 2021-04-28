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
#include<vector>
using namespace std;

SelelctCar::SelelctCar(ZKClient *zk):carNum(0),_zk(zk) {
}

string SelelctCar::getNodeVal(string msg) {

    string res_head ="selectCarFirstRes";
    string res_body;
    string res_head_info;
    string response_send_str;

    bool flag = true;

    CarRequest carRequest;
    carRequest.ParseFromString(msg);

    string longitude = carRequest.longitude();
    string latitude = carRequest.latitude();


    CarResponse carResponse;

    string path = _zk->getRootPath();
    path += "/经度"+longitude+"纬度"+latitude;
    vector<string> children = _zk->getNodeChildren(path.c_str());
    string area;

    for(int i = 0; i < children.size(); i++) {

        area = path + "/"+children[i];
        vector<int> vec = getOccupyCar(area);
        cout<<vec.size()<<endl;

         if(!vec.empty()) {

              flag = false; 
              carResponse.set_code(1);
              CarInfo * carInfo;
              carInfo =carResponse.add_info();
              carInfo->set_area(children[i]);
              for(int v : vec) {
                  carInfo->add_location(v);
              }

         }

    }

    if(flag) {
        carResponse.set_code(2);
    }

    carResponse.set_car_num(carNum);

    cout<<carNum<<endl;
    carResponse.SerializeToString(&res_body);


     Head head;
     head.set_head(res_head);
     head.SerializeToString(&res_head_info);
     int head_size = htonl(res_head_info.size());  // 转成大端
     response_send_str.insert(0, string((char*)&head_size, 4));

     return response_send_str+res_head_info+res_body;


}

    vector<int> SelelctCar::getOccupyCar(string msg) {

        vector<int> vec;

        vector<string> location = _zk->getNodeChildren(msg.c_str());

        carNum += location.size();


        for(int i = 0; i< location.size(); i++) {
             
            string msg_path;
            msg_path =msg + "/"+location[i];

           string val = _zk->getVal(msg_path.c_str(),1);

            if(val == "1") {

                vec.push_back(atoi(location[i].c_str()));

            }

        }

        return vec;

    }




