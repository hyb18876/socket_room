#include"fj.h"
#include"wl.h"
#include<string>
#include<string>
#include<string.h>
using namespace std;
bff* socket_khd;
class xyt
{
public:
    std::string to = "";
    void add(std::string p)
    {
    
        to+=p;
    }

};

 
//回应http GET请求
std::string sendfile(int index,char*fname)
{ 
    int size=30920;
    int filesize=size;
    xyt  to;//响应头
    if (index<1024) {
        char sizech[4];
        to.add("HTTP/1.1 200 OK\r\n");//回应一个状态，以\r\n作为分割
        to.add("Server: XHttp\r\n");         //服务器类型
        to.add("Content-Type: text/html;charset=utf-8\r\n");  //http协议支持各种类型的文件，html，pdf，音视频类型，所以这里告诉它类型
        to.add("Content-Encoding: gzip\r\n");
	    to.add("Content-Length: ");        //现在我们并不知道这个文件的大小
        sprintf(sizech,"%d",filesize);
        to.add(sizech);
        to.add("\r\n\r\n");                        //先写死加10个数据
                             
        printf("响应头size:%d\n",to.to.size());
        printf("响应体size:%d\n",filesize);
        printf("响应报文size:%d\n",to.to.size());
        socket_khd[index].s_set(to.to);
        send(index, socket_khd[index].s_char(), socket_khd[index].s_size(), 0);//发送协议头 
        char *ch=new char[size];
        FILE *sfp;
    char sfilename[FILENAME_MAX]="88.gzip";
    if((sfp = fopen(sfilename, "rb")) == NULL)
    {
    	 printf("\aSource file open failed.\n");
    }
    else
    {
        fread(ch, 1, size, sfp);
        send(index,ch,size,0);
        fclose(sfp);
    }
    }
    return to.to;
}
int main()
{
    //服务器架构
    //架构1. 
	//iohx(记录客户端ip及端口等等基本信息)
	//bff(客户端的收发缓冲器)
	//架构2.
	//采用selec模型监听客户端(并发)
	//基础socket(服务器)
	
	
	printf("http高并发服务器\n");
	int lj=0;
	int dklj=0;	
	iohx *p;                            //记录客户端(ip+port)
	fwq ai;                             //服务器 
	socket_khd=NULL;                    //客户端缓冲区 
	xz  at;                             //select模型
	ai.init("0.0.0.0", 8800);           //初始化服务器
	at.xz_init(&p,&socket_khd,ai.jk);   //初始化select模型
	int index = 0;
	while (true)
	{   	
	printf("连接个数%d\n",lj);
	printf("断开连接个数%d\n",dklj);
		at.xz_qd(&ai);//启动select模型 放入服务器
		//xz_qd()执行完了会更新三个表 jh ，hx，khdxy，
		//hx 是记录客户端套接字的基本信息(客户端消息的读写从该表操作）
		//jh是服务器框架监听所有套接字
		//khdxy 监听的客户端响应的集合	
		for (int i = 0; i < at.khdxy.fd_count; i++)//遍历响应的客户端套接字
		{ 
			    index= at.khdxy.fd_array[i];//套接字
				printf("状态码%d ", p[index].bf);//状态码//0 连接 1//数据 //2 断开连接
				printf("id:%d   addr:%s:%d  \n",index, p[index].ip, p[index].port);//ip:port
				if (p[index].bf == 0)
				{
					lj++;
				}
				else if (p[index].bf == 1)
				{	 // printf("请求报文:%s\n",socket_khd[index].r_char());
				     sendfile(index,"88.gzip"); 
				     //break;
			     }
				else if(p[index].bf==2)
				{
				    dklj++; 
					closesocket(index);//关闭套接字
				}	
		}
		printf("\n");
	}
	at.xz_colse(ai.jk);
	return 0;
}
