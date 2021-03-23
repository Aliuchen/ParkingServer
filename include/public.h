/*===============================================================
*   Copyright (C) 2021 All rights reserved.
*   
*   文件名称：public.h
*   创 建 者：@liuchen
*   创建日期：2021年01月31日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once
#include<iostream>
#include<stdio.h>

using namespace std;

#define LOG(str) \
    cout<< __FILE__ << ":" << __LINE__ << " " \
    << __TIMESTAMP__ << ":" << str << endl;
