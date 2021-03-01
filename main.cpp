#include<iostream>
#include<thread>
#include  <unistd.h> 
#include  <fcntl.h> 
#include  <errno.h>
#include  <sys/types.h>
#include  <sys/socket.h> 
#include  <sys/epoll.h> 
#include  <netinet/in.h> 

#include  <arpa/inet.h> 
#include<error.h>
#include<map>
#include<string.h>
#include"include/EPoll_op.h"
// struct d_data{
// char *data;
// int fd;

// };

int main(int argc,char *argv[])
{
 EPoll_op eop;
 eop.EPollCircle();

  //   sockaddr_in sin,clientsin;int clientlen=sizeof(clientsin);
  //   sin.sin_family=AF_INET;

  //   sin.sin_addr.s_addr=INADDR_ANY;
  //   sin.sin_port=htons(2021);
    
  //   int listenfd=socket(AF_INET,SOCK_STREAM,0);
  //   int eplfd=epoll_create(20);
  //   epoll_event eventp,waitevent;
  //   eventp.events=EPOLLIN|EPOLLET;
  //   int z=0;

  //   eventp.data.fd=listenfd;
  //   epoll_ctl(eplfd,EPOLL_CTL_ADD,listenfd,&eventp);
  //   bind(listenfd, (sockaddr*)&sin,sizeof(sockaddr_in));
  //       std::cout<<"hello service";
  // // fcntl(listenfd,F_SETFL,O_NONBLOCK);
  //   while(listen(listenfd,20)==-1)
  //   {
  //     listen(listenfd,20);

  //   }
  //   std::cout<<"成功";
  //   while(true)
  //   {
  //   int counts= epoll_wait(eplfd,&waitevent,2,-1);
  //   for(int i=0; i<counts;i++)
  //   {
  //      if((&waitevent)[i].data.fd==listenfd)
  //      {
  //          int client_t=accept(listenfd,reinterpret_cast<sockaddr*>(&clientsin),(socklen_t*)&clientlen);
  //          char *addr=inet_ntoa(clientsin.sin_addr);
  //         std:: cout<<"addr is"<<addr;
  //        fcntl(client_t,F_SETFL,O_NONBLOCK);
  //         eventp.data.fd=client_t;
  //         eventp.events=EPOLLET|EPOLLIN;
  //         epoll_ctl(eplfd,EPOLL_CTL_ADD,client_t,&eventp);
  //      }
  //      else if((&waitevent)[i].events==EPOLLIN)
  //      {
  //          int client=waitevent.data.fd;
  //          char buff[21];
     
    
  //          int  re=read(client,buff,20);
  //           std::cout<<buff;
  //          z=re;
  //            while(re>0)
  //          {    
  //              re=read(client,buff,20);
  //              z+=re;
          
  //   }
  //        std::cout<<z<<buff;
  //       d_data *ss=new d_data;
  //       ss->fd=(&waitevent)[i].data.fd;
  //       ss->data=new char[30];
  //       strcpy(ss->data,"hellow");

  //      eventp.data.ptr=(void*)ss;
  //      eventp.events=EPOLLOUT|EPOLLET;
  //      epoll_ctl(eplfd,EPOLL_CTL_MOD,client,&eventp);
         
  //      }
  //      else if((&waitevent)[i].events==EPOLLOUT)
  //      {
  //          d_data *d=static_cast<d_data*>((&waitevent)[i].data.ptr);
  //          int clientfdout=d->fd;
  //          write(clientfdout,d->data,strlen(d->data));
  //          eventp.data.fd=clientfdout;
  //          eventp.events=EPOLLIN|EPOLLET;
  //          epoll_ctl(eplfd,EPOLL_CTL_MOD,clientfdout,&eventp);
           
         
  //      }
  //   }
    
  //   }

//    std::thread thobj(testthread);
//    thobj.join();

    return 0;
}