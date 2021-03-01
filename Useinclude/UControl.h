#pragma once
#include"../include/LitControl.h"
class UControl:public LitControl{
protected:
void Initialize();//初始化使用detch函数分配信号和函数
void litcontrol(std::string,Storage);//未分配的信号皆在此地
public:
UControl(EPoll_op*,int);//初始化
void testresponse(Storage);
void toother(Storage);
// void runfunction(void(UControl::*function)(Storage),Storage s);
};
// void UControl::runfunction(void(UControl::*function)(Storage),Storage s)//保留不可删除
// {
//     (this->*function)(s);
// }
