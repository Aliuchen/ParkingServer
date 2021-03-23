/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：FindPasswd.cpp
*   创 建 者：@liuchen
*   创建日期：2021年02月15日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"FindPasswd.h"
Mail* Mail::getMailInstance() {

    static Mail mail;
    return &mail;
}

Mail::Mail() {

//    _fp = popen("echo 邮件服务开启 | mailx  -s \"demo title\"  1328749214@qq.com ","r");
     _fp = popen(" ","r");
    if(!_fp) {

        cout<<"++++"<<endl;
    }
}
Mail::~Mail() {

    pclose(_fp);
}

bool Mail::sendMessage( string str) {

    cout<<str.c_str()<<endl;
    int error = fprintf(_fp,str.c_str());
    cout<<error<<endl;
    
    if(error < 0) {
        return false;
    }
    return true;
}
int main()
{

    Mail * mail = Mail::getMailInstance();

    mail->sendMessage("echo 123456 | mailx  -s \"demo title\"  1328749214@qq.com");
    cout<<endl;
    return 0;
}
