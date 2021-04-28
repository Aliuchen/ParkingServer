/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：LoadToZk.cpp
*   创 建 者：@liuchen
*   创建日期：2021年03月21日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"LoadToZk.h"
#include<vector>

using namespace std;

LoadToZK::LoadToZK() {

    _zkclient.start();
     _pool = ConnectionPool::getConnectionPool();
      _connection = _pool->getConnection();

}

void LoadToZK::start() {

    string sql = "select longitude,latitude,carport_info from carport";

    MYSQL_ROW row;
    MYSQL_RES *res = _connection->query(sql);
    string path = _zkclient.getRootPath();
    _zkclient.create(path.c_str(),nullptr,-1);

    if(res == nullptr) {

        cout<<"db 中没有停车信息"<<endl;

    }else {

        while((row=mysql_fetch_row(res))!=NULL) {

        string _long = row[0];
        string _lat = row[1];
        path +="/";

        path +="经度"+_long+"纬度"+_lat;

        Json::Reader reader;
        Json::Value value;

        string myjson = row[2];
        myjson = myjson.substr(1,myjson.length()-2);
        reader.parse(myjson,value);
    
        Json::Value::Members me = value.getMemberNames();

        _zkclient.create(path.c_str(),nullptr,-1);
        
        string data = to_string(0);
        for(auto it = me.begin(); it != me.end(); it++) {

            string areaPath = path + "/" + (*it).c_str();
            _zkclient.create(areaPath.c_str(),nullptr,-1);

            string endPath = areaPath + "/" + to_string(1);
           
          
            int size = value[(*it).c_str()].asInt();
            for(int i = 0; i < size; i++) {
                string endPath = areaPath + "/" + to_string(i+1);
                _zkclient.create(endPath.c_str(),data.c_str(),data.size(),ZOO_EPHEMERAL);
            }
         
        }
       

        }

    }

    

    
  
    string valPath = "/ParkingService/经度108.95468917825438纬度34.22987464828418/D/5";

    
 //   string val = _zkclient.getVal(valPath.c_str(),1);

  //  cout<< val<<"   "<<val.size()<<endl;

    
    
    _zkclient.setVal(valPath.c_str(),to_string(1).c_str(),to_string(1).size());
/*
    val = _zkclient.getVal(valPath.c_str(),1); 
     cout<< val<<val.size()<<endl;

    */
    /*


    string s = "/ParkingService/经度108.95468917825438纬度34.22987464828418";
   
    cout<<path<<endl;
    vector<string> vec = _zkclient.getNodeChildren(s.c_str());

    for(auto it : vec) {

        cout<<it<<endl;
    }
*/    
   

}
