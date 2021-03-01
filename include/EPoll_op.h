#pragma once
#include<mutex>
#include<map>
#include<vector>
#include"afx_head.h"
#include<string>
#include<string.h>
class UControl;
struct eventptr{//保存数据
private:
long offset=0;//已读或写偏移
long length=sizeof(head);//需要读或写的长度
bool rest=false;//是否为内容
char datahead[sizeof(head)];

public:
int fd;//套接字标识符
Storage sto;//数据
UControl *control;//发送数据
long getrest()//返回需要发送或接收的类
{
    return length-rest;
}
void adddata(char *data_p,int s);
~eventptr(){
    if(rest)
    {
        delete[] sto.content;//删除收到一半的数据 避免成为野指针
    }
}
};
// struct Control{
// char *buff=nullptr;//保存内容指针
// char *head=nullptr;//保存信号头指针
// bool state=false;//false表示接收的是头 true接收的是头
// int reserve=sizeof(head);//剩余需要接收的信息
// LitControl*control;//control指针
// Control(EPoll_op*parent,int fd)
// {
//     control=new LitControl(parent,fd);
// }
// };
struct W_data{

    int length=0;
    char*data=nullptr;
};
struct Sendll
{
 
    Storage sto;
    char headptr[sizeof(head)];//保存原有数据的指针
    int offset=-1;
    int length=sizeof(head);
    bool bset=false;//标记是否读至内容
   bool bret=true;//是否可直接写
   UControl *control;

   W_data getdata();//直接获取数据
   bool getstate()
   {
       if(offset==-1)
       return true;
       return false;
   }
   void setoffset(long sendcount)//设置偏移
   {

       if(sendcount>0)
      
    {  offset+=sendcount;
      if(offset==length&&!bset)
      {
      bset=true;
      offset=0;
      length=sto.length;

      }
      else if(offset==length&&bset)
      {
          bset=false;
          offset=-1;
          length=sizeof(head);
          
          delete[] sto.content;
          memset(&sto,0,sizeof(Storage));

      }
   }}


    
};

class EPoll_op
{
private:
std::mutex mux;//互斥变量
std::mutex mux_send;//发送标识符互斥变量
std::mutex mux_broadcast;//广布互斥变量
int epollfd;//epoll标识符
// int epolloutfd;//epoll发送标识符
int listenfd;//监听套接字标识符

std::map<std::string,int>Broadcast;//广播依赖的map容器 保存字符标识和套接字标识符 用于广播
std::map<int,UControl*>ControlList;//控制层容器，根据int标识符找到Litcontrol指针
std::map<int,Sendll*>Sendclient;//发送区非阻塞，所以保存未发送完的数据
std::map<int,eventptr*>eventptrlist;//保存发送数据
//std::vector<Storage*>Broadlist;
public:
EPoll_op();//构造函数，直接初始化epoll  listenfd;
void LittleSettitle(int clientfd,char *name);//设置标识符
void EPollCircle();//事件循环
void BroadCast(Storage,int fd);//broadcast向每个control的发送缓冲区尾部添加发送事务，不包含己身
void BroadCasttoPerson(char*,Storage);//向特定某个人发送消息
void Sendstate(int client,UControl*);//设置发送状态为待发送
};