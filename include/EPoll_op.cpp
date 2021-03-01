#include"EPoll_op.h"
#include  <unistd.h> 
#include  <fcntl.h> 
#include  <errno.h>
#include  <sys/types.h>
#include  <sys/socket.h> 
#include  <sys/epoll.h> 
#include  <netinet/in.h> 
#include"../Useinclude/protocal.h"
#include  <arpa/inet.h> 
#include<iostream>
#include"../Useinclude/UControl.h"
EPoll_op::EPoll_op()
{

listenfd=socket(AF_INET,SOCK_STREAM,0);
sockaddr_in sin;
sin.sin_family=AF_INET;
sin.sin_addr.s_addr=INADDR_ANY;
sin.sin_port=htons(port);
int len=sizeof(sockaddr_in);
bind(listenfd,reinterpret_cast<sockaddr*>(&sin),sizeof(sockaddr_in));
fcntl(listenfd,F_SETFL,O_NONBLOCK);//设置为非阻塞
listen(listenfd,20);//监听 listenfd初始化完成

epollfd=epoll_create(30);
epoll_event event;
eventptr *ptr=new eventptr;
ptr->fd=listenfd;
event.data.ptr=static_cast<void*>(ptr);
event.events=EPOLLIN|EPOLLET;
epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&event);//epollfd初始化完成

}
void EPoll_op::EPollCircle()
{
    epoll_event waitevent[20];

   while(true)
   {
   int count= epoll_wait(epollfd,waitevent,20,20);

      for(int i=0;i<count;i++)
   {
       eventptr*tmp= static_cast<eventptr*>(waitevent->data.ptr);
        if(tmp->fd==listenfd)//有新链接接入
    {
    sockaddr_in sin;
    int len=sizeof(sin);
    int client=accept(listenfd,reinterpret_cast<sockaddr*>(&sin),reinterpret_cast<socklen_t*>(&len));
    fcntl(client,F_SETFL,O_NONBLOCK);
    epoll_event evento;  
    eventptr *newptr=new eventptr;//创建事件数据
     UControl *control_m=new UControl(this,client);//创建control
     control_m->start();
    newptr->control=control_m;
    newptr->fd=client;
    evento.events=EPOLLIN|EPOLLET;
    evento.data.ptr=static_cast<void*>(newptr);
   int s= epoll_ctl(epollfd,EPOLL_CTL_ADD,client,&evento);
  
   ControlList.insert(std::pair<int,UControl*>(client,control_m));//插入控制列表
  eventptrlist.insert(std::pair<int,eventptr*>(client,newptr));//插入数据维护列表

  std::cout<<s;
    }
    else if(waitevent[i].events&EPOLLIN)
    {
      int client=tmp->fd;
       long rest=tmp->getrest();
      char data[rest];
      int res=0;
      while((res=read(client,data,rest))>0)
      {
        tmp->adddata(data,res);//内部拆包合包 发送

      }
    
             if(res==0||(res<0&&errno!=EAGAIN))
          {
              close(client);
              
              delete tmp;//断开连接销毁 内部自动销毁content
              mux_send.lock();
              if(Sendclient.find(client)!=Sendclient.end())
              Sendclient.erase(Sendclient.find(client));
              mux_send.unlock();
            
              mux_broadcast.lock();
              for(auto i=Broadcast.begin();i!=Broadcast.end();i++)
              {
                if(i->second==client)
                {
                  Broadcast.erase(i);
                  break;
                }
              }
               mux_broadcast.unlock();
                  if(eventptrlist.find(client)!=eventptrlist.end())
              eventptrlist.erase(eventptrlist.find(client));
             
              epoll_ctl(epollfd,EPOLL_CTL_DEL,client,nullptr);
                if(ControlList.find(client)!=ControlList.end())
            {  
              ControlList.at(client)->Setexit();//通知退出
              ControlList.erase(ControlList.find(client));
              }

          }
    }
    else if(waitevent[i].events&EPOLLOUT)
    {
      int client=tmp->fd;
       auto o=Sendclient.find(client);
       if(o!=Sendclient.end())
       {
         o->second->bret=true;//设为可读
       }
       epoll_event event;
       event.data.ptr=static_cast<void*>(tmp);
       event.events=EPOLLIN|EPOLLET;
       
      epoll_ctl(epollfd,EPOLL_CTL_MOD,client,&event);

    }
      

  }

     for( auto it=Sendclient.begin();it!=Sendclient.end();)//遍历发送队列
   {
      Sendll *opra=it->second;
      if(opra->bret)//如果未满
  {
     do{
             W_data w= opra->getdata();
           if(w.length==0)
           {
             
            
             it=Sendclient.erase(it);
              delete opra;//销毁
            if(it!=Sendclient.begin())
            it--;
            break;
           }
           else
          { 
            char data[w.length];
           memcpy(data,w.data,w.length);
           int res=write(it->first,data,w.length);//发送
           if(w.length==204)
           {
             head test;
             memcpy(&test,data,w.length);
             int sspps=test.length;
           }
           opra->setoffset(res);
           if(res==-1&&errno==EAGAIN)
           {
             opra->bret=false;
             epoll_event ev;
             ev.events=EPOLLIN|EPOLLOUT|EPOLLET;
             ev.data.ptr=static_cast<void*>(eventptrlist.at(it->first));
             epoll_ctl(epollfd,EPOLL_CTL_MOD,it->first,&ev);
             break;
           }
           }
}while(!opra->getstate());
           }
    if(it!=Sendclient.end())
    it++;
   }
   }

}
void EPoll_op::LittleSettitle(int fd,char*name)
{
    using namespace std;
    std::map<std::string,int>::iterator mapt;
 mapt=Broadcast.find(name);
 std::map<std::string,int>::iterator i;
 lock_guard<mutex>s(mux_broadcast);
 for(i=Broadcast.begin();i!=Broadcast.end();)//查找是否有fd若有则删除
 {
     if(i->second==fd)
     {
        i= Broadcast.erase(i);
      
     }
     if(i!=Broadcast.end())
     {
        i++;
     }
     
 }
 if(mapt==Broadcast.end())
 {
     Broadcast.insert(std::pair<std::string,int>(name,fd));
 }
 else{
     Broadcast.at(name)=fd;
 }

}
void EPoll_op::Sendstate(int fd,UControl*control)
{
    Sendll *s=new Sendll;
    s->control=control;
    std::lock_guard<std::mutex>sp(mux_send);
   Sendclient.insert(std::pair<int,Sendll*>(fd,s));
}
void EPoll_op::BroadCast(Storage s,int fd)//broadcast向每个control的发送缓冲区尾部添加发送事务，不包含己身
{

    using namespace std;
    map<int,UControl*>::iterator i;
for(i=ControlList.begin();i!=ControlList.end();i++)//往每个发送缓冲区中加
{
  if(i->first!=fd)
    ControlList.at(i->first)->LitSend(s);
}
s.release();//释放content
}
void EPoll_op::BroadCasttoPerson(char*name,Storage s)//向特定某个人发送消息
{
 using namespace std;
 map<string,int>::iterator it;
map<int,UControl*>::iterator its;
  it= Broadcast.find(name);
  if(it==Broadcast.end())
  {
      return;
  }
  else{
      int client=it->second;
       its=ControlList.find(client);
      if(its!=ControlList.end())
      {
       
        its->second->LitSend(s);//缓冲队列添加
      }else
      {
        s.release();
      }
  
    //   ControlList
  }
}
void eventptr::adddata(char *data_p,int s)//自动添加函数
{
   if(!rest)
   {
       memcpy(datahead+offset,data_p,s);
       offset+=s;
   }
   else
   {
       mempcpy(sto.content+offset,data_p,s);
       offset+=s;
   }
   if(offset==length)
   {
     if(rest)
     {
         rest=false;
         offset=0;
         length=sizeof(head);
      
         control->Recv(sto);//发送信号
         memset(&sto,0,sizeof(Storage));//清理数据
     }
     else{
         head htm;
         memcpy(&htm,datahead,sizeof(head));
         offset=0;
         length=htm.length;
         strcpy(sto.signal,htm.signal);
         sto.content=new char[length];//申请内存
         sto.length=length;
         rest=true;
     }

   }


}
W_data Sendll::getdata()
{
  if(offset==-1)
  {
    
   Storage *ts=control->getSendData();
   if(ts==nullptr)
   {
     W_data w;
     
      return w;
   }
    else
    {
      head htm;
      
      sto=*ts;
      htm.length=sto.length;
      memcpy(htm.signal,sto.signal,200);

      delete ts;//释放内存
      offset=0;
      memcpy(headptr,&htm,sizeof(head));
      W_data w;
      w.length=length;
      w.data=headptr;
      return w;
    
    }

  }
  else{
    W_data w;
    if(bset)
    {
      w.data=sto.content+offset;
      w.length=length-offset;
     
    }
    else{
      w.data=headptr+offset;
      w.length=length-offset;
      
    }
     return w;
  }

}