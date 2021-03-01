#pragma once
#include<map>
#include<vector>
#include<mutex>
#include<string.h>
#include"afx_head.h"
#include"EPoll_op.h"
#include<thread>
class UControl;
class LitControl{
private:
int sockfd;//套接字用以发送时找到事件
UControl*child;
EPoll_op*parent;//底层epoll，用于settitle
std::map<std::string,void(UControl::*)(Storage)>AllocfunctionList;//保存字符串跟函数指针的对应
std::vector<Storage*>Sendlist;//保存发送的数据
std::vector<Storage>Recvlist;//保存接收的数据
std::thread* thread_m;
std::mutex mux_send;
std::mutex mux_recv;

bool exitb=false;
protected:
void deptach(std::string,void(UControl::*function)(Storage));//用于存储字符串跟函数的关系

void Settitle(char*);//设置套接字用户对应名，用与广播
void clear();//清除内存
virtual void Initialize();//初始化allocfunctionlist
void Initializeself(UControl*);//将子对象发送至父类
virtual void deletemy();//删除自己
// virtual void runfunction(void(UControl::*function)(Storage),Storage s);
void DealCircle();//内部处理接收事件
virtual void litcontrol(std::string,Storage);//重写，并处理事件
void LitSend(char*signal,char*data,int length);//用于发送char流数据
void Broadcast_m(char *signal,char*content,long length);//广播流数据
void Broadcasttoperson_m(char*person,char *signal,char*content,int length);//广播流数据
template<typename T>
void LitSend(char *,T*);//发送数据，构造Storage对象，保存在Sendlist里
template<typename T>
void LitEncode(char *,T*);//解码
template<typename T>
void Broadcast_m(char *,T);//向所有人广播

template<typename T>
void Broadcasttoperson_m(char*person,char*signal,T);


public:
void Recv(Storage);
void dispatchdata(Storage);//分发数据
Storage *getSendData();//获取发送队列中的数据
void LitSend(Storage);//用于epoll发送广播数据
LitControl(EPoll_op*,int);//初始化获取epoll_op指针，SOCKFD标识符
void Setexit();
void start();//开始线程

// void testresponse(Storage);
};
template<typename T>
void LitControl::LitSend(char *signal,T*data)//发送数据，构造Storage对象，保存在Sendlist里
{
Storage *st_tmp=new Storage;
char tmp_t[sizeof(T)];
memcpy(tmp_t,data,sizeof(T));
strcpy(st_tmp->signal,200);
strcpy(st_tmp->content,tmp_t,sizeof(T));
st_tmp->length=sizeof(T);

mux_send.lock();
Sendlist.push_back(st_tmp);
mux_send.unlock();
if(Sendlist.size()==1)
{
    parent->Sendstate(sockfd,child);//发送就绪
}
}
template<typename T>
void LitControl::LitEncode(char *raw,T*data)//解码
{
     memcpy(data,raw,sizeof(T));

}
template<typename T>
void LitControl::Broadcast_m(char *data,T t)//向所有人广播
{
    Storage s;
    strcpy(s.signal,data);
    s.content==new char[sizeof(T)];
    mempcpy(s.content,&t,sizeof(T));
    s.length=sizeof(T);

  
parent->BroadCast(s,sockfd);
}
template<typename T>
void LitControl:: Broadcasttoperson_m(char*person,char*signal,T t)
{
       Storage s;
    strcpy(s.signal,signal);
    s.content==new char[sizeof(T)];
    mempcpy(s.content,&t,sizeof(T));
    s.length=sizeof(T);
    parent->BroadCasttoPerson(person,s);
}