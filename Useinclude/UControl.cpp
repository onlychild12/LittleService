#include"UControl.h"
#include<iostream>
UControl::UControl(EPoll_op*eop,int fd):LitControl(eop,fd)//初始化
{
Initializeself(this);//不可删除！
}

void UControl::Initialize()//使用detch函数分配信号和函数
{
    
    
// deptach("test",&UControl::testresponse);
// deptach("file",&UControl::toother);


}
void UControl::litcontrol(std::string,Storage)//未分配的信号皆在此地
{

}
// void UControl::testresponse(Storage s)
// {
//     LitSend("recv","love",4);
//   Settitle(s.content);
// }

// void UControl::toother(Storage s)
// {
      
//       Broadcasttoperson_m("test1","file",s.content,s.length);
//     // //    LitSend("file",tmp,s.length);

//     // Broadcast_m("file",tmp,s.length);
// }
