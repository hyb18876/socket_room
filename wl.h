#pragma once
#include <stdio.h>  
#include <winsock2.h>  
#pragma comment(lib,"ws2_32.lib")  
#pragma warning(disable:4996)
#include<string>
#include<string>
#include<string.h>
class bff//socket 缓冲区管理器 
{
public:

std::string s_data="";//发送缓冲区(s)
std::string r_data="";//接收缓冲区(r)
char* r_char(){return (char*)r_data.c_str();}
char* s_char(){return (char*)s_data.c_str();}
void s_set(std::string p){s_data=p;}//设置s缓冲区内容
int s_size(){return s_data.size();}//获取s缓冲区内容size
int r_size(){return r_data.size();}//获取r缓冲区内容size
void s_memset(){s_data="";}//设置s缓冲区内容为空
void r_memset(){r_data="";}//设置r缓冲区内容为空
void add(std::string p){s_data+=p;}//s缓冲区添加内容
void socket_send(int id){}//发送消息给id
void socket_recv(int id){printf("复制的文本:%s\n",r_char());}//接收id的消息
};//客户端列表(id)
class iohx//socket 基础信息表 
{
  public:
	char*ip;//ip地址
	int port;//端口
	int bf=0;//状态码//0 连接 1//数据 //2 断开连接
	
	void set(char*ip,int port,int bf )//连接时候调用 
	{this->bf = bf;
		this->ip = ip;
		this->port = port;
	}
	void set(int bf)//设置socket状态 
	{
		this->bf = bf;
	}
};
class fwq//服务器类
{
public:
	sockaddr_in add[1024];//各客户端IP
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	SOCKET jk;//服务器套接字
	int  init(const char *p, int port)
	{
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(sockVersion, &wsaData) != 0)
		{
			return 0;
		}
		socket_cj();//创建套接字
		bind_socket(p, port);//绑定
		if (listen(jk, 100) == SOCKET_ERROR)//设置监听队列
		{
			printf("listen error !");
			return 0;
		}
		return 1;
	}
	int socket_lj()
	{
		int index = accept(jk, (SOCKADDR *)&remoteAddr, &nAddrlen);//执行连接
		add[index].sin_addr = remoteAddr.sin_addr;
		add[index].sin_port = remoteAddr.sin_port;
		return  index;
	}
	int	socket_cj()
	{
		//创建套接字  
		jk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (jk == INVALID_SOCKET)
		{
			printf("socket error !");
			return 0;
		}
		return 1;
	}
	int bind_socket(const char *p, int port)
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		if (*p == NULL)
		{
			sin.sin_addr.S_un.S_addr = INADDR_ANY;
		}
		else
		{

			sin.sin_addr.S_un.S_addr = inet_addr(p);
		};
		if (bind(jk, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			printf("bind error !");
			return 0;
		}
		return 1;
	}


};
class xz //select 模型类 
{
public:
	bff khd[1024];
	char http_data[1024*10];
	SOCKET sClient;//临时记录客户端
	iohx hx[1024];//记录客户端ip 状态 
	fd_set jh;//创建监听集合
	fd_set jh2;//被内核修改的集合(jh的响应集合)
	fd_set khdxy;//客户端套接字集合
	void xz_init(iohx**p,bff**pp, int jk)
	{
	    *pp=khd;
		*p = hx;
		FD_ZERO(&jh);//重置集合
		FD_SET(jk, &jh);

	}
	void xz_qd(fwq*p)
	{
		int ms=0;
		FD_ZERO(&khdxy);//重置集合
		jh2 = jh;
		for (int i = 1; i < jh.fd_count; i++)//遍历监听集合
		{
			int index = jh.fd_array[i];
			//printf("addr:%s:%d  socket:%d\n", inet_ntoa(p->add[index].sin_addr), p->add[index].sin_port, index);
		}
		//临时集合
		select(NULL, &jh2, NULL, NULL, NULL);//监听集合
		for (int i = 0; i < jh.fd_count; i++)//遍历监听集合
		{

			if (FD_ISSET(jh.fd_array[i], &jh2))//该套接字是否在回响应集合中
			{
				
				//把响应的套接字返回到使用者手上
				if (jh.fd_array[i] == p->jk)//服务器套接字响应
				{
					
					sClient =p-> socket_lj();//执行连接	
					hx[sClient].set(inet_ntoa(p->remoteAddr.sin_addr), p->remoteAddr.sin_port, 0);
					FD_SET(sClient, &jh);//客户端套接字添加监听集合
					FD_SET(sClient, &khdxy);
				}
				else//客户端套接字响应
				{
					int index = jh.fd_array[i];	
					FD_SET(index, &khdxy);
if(ms==0)
{
                  
					memset(http_data,0,sizeof(http_data));
					if (recv(jh.fd_array[i], http_data, sizeof(http_data), 0) > 0)//客户端接收数据
					{   
							hx[jh.fd_array[i]].set(1);//设置socket状态 
							//strcpy(khd[index].r_char(),http_data);//拷贝到客户端管理器(缓冲区) 
					}else
					{
						hx[jh.fd_array[i]].set(2);//设置socket状态
						closesocket(jh.fd_array[i]);//关闭套接字
						FD_CLR(jh.fd_array[i], &jh);//从监听集合中删除该套接字

					}
}
				}
			}
		}
		

	}
	void xz_colse(int jk)
	{
		for (int i = 1; i < jh.fd_count; i++)//遍历监听集合
		{
			int index = jh.fd_array[i];
			closesocket(index);
		}
		closesocket(sClient);
		closesocket(jk);
		WSACleanup();
	}

};
