#include"LitControl.h"
#include"../Useinclude/UControl.h"
#include<iostream>
using namespace std;
LitControl:: LitControl(EPoll_op*eop,int clientfd)//初始化相关变量
{
parent=eop;


sockfd=clientfd;

}
void LitControl::Settitle(char*name)
{
    parent->LittleSettitle(sockfd,name);
}
void LitControl::Broadcasttoperson_m(char*person,char *signal,char*content,int length)//广播流数据
{
        Storage s;
    strcpy(s.signal,signal);
    s.content=new char[length];
    memcpy(s.content,content,length);
    s.length=length;  
    parent->BroadCasttoPerson(person,s);
}
void LitControl::Broadcast_m(char *signal,char*content,long length)
{
   Storage s;
    strcpy(s.signal,signal);
    s.content=new char[length];
    memcpy(s.content,content,length);
    s.length=length;  
    parent->BroadCast(s,sockfd);
}
void LitControl::start()//开始线程
{
Initialize();
thread_m=new thread(&UControl::DealCircle,child);//循环开始
thread_m->detach();
}
void LitControl::litcontrol(std::string name,Storage s)
{
    if(name=="test")
    {
    std::cout<<s.content;
    LitSend("recvtest","ffff",4);
    }
   if(name=="file")
   {
       char *tmp=new char[s.length];
       memcpy(tmp,s.content,s.length);
       LitSend("recvtest",tmp,s.length);
   }
}
 void LitControl::Initialize()
 {

    //   deptach("test",&LitControl::testresponse);
 }
void LitControl::deptach(std::string name,void (UControl::*function)(Storage))
{
if(AllocfunctionList.find(name)!=AllocfunctionList.end())
{
    AllocfunctionList.at(name)=function;
}
else
{
    AllocfunctionList.insert(pair<string,void(UControl::*)(Storage)>(name,function));
}
map<string,void(*)(Storage)>::iterator it;
// for(it=AllocfunctionList.begin();it!=AllocfunctionList.end();it++)
// {
//   if(it->second==function)
//   {
//       it=AllocfunctionList.erase(it);
//       it--;
//   }
// }



}
// void LitControl::runfunction(void(UControl::*function)(Storage),Storage s)
// {

// }
void LitControl::Initializeself(UControl*p)
{
    child=p;
}
void LitControl::DealCircle()
{
     while(true)
     {
      
         if(exitb==true&&Recvlist.size()==0)
         {
           break;
         }
         
     for(int i=0;Recvlist.size()!=0;)
     {

         Storage s=Recvlist.at(i);
         if(AllocfunctionList.find(s.signal)!=AllocfunctionList.end())
         {
           void(UControl::*dealfuntion)(Storage)=  AllocfunctionList.at(s.signal);
          (child->*dealfuntion)(s);
         }
         else{
             litcontrol(s.signal,s);
         }
         mux_recv.lock();
         
         Recvlist[0].release();//释放storage
         Recvlist.erase(Recvlist.begin());
         
         mux_recv.unlock();
     }
   
    
     }
    deletemy();//删除自己
}
void LitControl::Setexit()//设置线程退出
{
    exitb=true;

}
Storage* LitControl::getSendData()//获取数据
{
   if(Sendlist.size()==0)
   return nullptr;
   else{
       Storage *t=Sendlist.at(0);
       mux_send.lock();
       Sendlist.erase(Sendlist.begin());
       mux_send.unlock();
     return t;
     

   }
}
void LitControl::Recv(Storage s)
{
    lock_guard<mutex>ll(mux_recv);
     Recvlist.push_back(s);
}
void LitControl::LitSend(char*signal,char*data,int length)//用于流发送
{
    Storage *s=new Storage;
    s->content=new char[length];
    strcpy(s->signal,signal);
    memcpy(s->content,data,length);
    s->length=length;
        mux_send.lock();
    Sendlist.push_back(s);
    mux_send.unlock();
    if(Sendlist.size()==1)
{
    parent->Sendstate(sockfd,child);//发送就绪
}
}
void LitControl::LitSend(Storage s)//
{
    Storage *t=new Storage;
    *t=s;
    t->content=new char[s.length];
  
    memcpy(t->content,s.content,s.length);
     
    mux_send.lock();
    Sendlist.push_back(t);
    mux_send.unlock();
    if(Sendlist.size()==1)
     parent->Sendstate(sockfd,child);

}
void LitControl::deletemy()//删除自己
{
    clear();
    delete child;//删除子类
}
void LitControl::clear()
{
  for(int i=0;i<Sendlist.size();i++)
  {
      delete Sendlist[i];
  }
  Sendlist.clear();
    Recvlist.clear();
}
