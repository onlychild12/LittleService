#pragma once
struct Storage{
int length;//标注content即内容的大小
char signal[200];
char*content;
template<typename T>
bool LitEncode(T*data)//解码
{
     if(sizeof(T)!=length)//如果大小不匹配直接返回null
     {
         data=nullptr;
         return false;
     }
     else{
     data=new T;
     memcpy(data,content,sizeof(T));
     delete[] content;//释放指针内存
     return true;

     }

}
void release()
{
    delete[] content;//释放缓冲区空间
}
};
struct head{
int length;
char signal[200];
};
