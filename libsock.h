//	SkyFire 2016-04-07	skyfireitdiy@hotmail.com
//简介：libsock为了简化socket编程



//	更新日志：
//	V1.1：
//		1.回调函数添加otherParam，方便参数传递
//			


//	使用方法：
//		TCP服务端面向过程：
//			SockInit()初始化
//			MakeSocket()创建套接字
//			BindSocket()绑定套接字
//			实现TCP连接回调函数（发送与接收在这里实现）
//			ListenSocket()监听套接字
//			使用结束调用CloseSock()
//			SockClean()清理
//
//		TCP客户端面向过程：
//			SockInit()初始化
//			MakeSocket()创建套接字
//			BindSocket()绑定套接字
//			TCPConnect()连接服务器
//			发送接收等操作
//			使用结束调用CloseSock()
//			SockClean()清理
//
//		UDP面向过程：
//			SockInit()初始化
//			MakeSocket()创建套接字
//			BindSocket()绑定套接字
//			发送接收等操作
//			使用结束调用CloseSock()
//			SockClean()清理
//
//		TCP服务器面向对象：
//			实现TCP连接回调函数、TCP接收回调函数、TCP关闭回调函数
//			实例化TCPServer对象
//			调用init()方法
//			调用listen()方法
//			使用结束调用close方法
//
//		TCP客户端面向对象：
//			实现TCP连接回调函数、TCP接收回调函数、TCP关闭回调函数
//			实例化TCPClient对象
//			调用init()方法
//			调用connect()方法
//			使用结束调用close方法	
//
//		UDP面向对象：
//			实现UDP接收回调函数
//			实例化UDPSocket对象
//			调用init()方法
//			调用recv()方法
//			使用结束调用close方法

#ifndef LIBSOCK_H_
#define LIBSOCK_H_

#ifdef LIBSOCK_EXPORTS
#define LIBSOCK_API __declspec(dllexport)
#else
#define LIBSOCK_API __declspec(dllimport)
#endif


#include <windows.h>


enum LIBSOCK_API CONNECTTYPE {			//连接协议类型
	TCP,								//TCP连接
	UDP									//UDP连接
};

enum LIBSOCK_API SOCKERR {			//接收错误类型返回值
	NOERR,								//没有错误，接收到消息
	NOMESSAGE,							//没有消息可以接收
	DISCONNECT							//连接已断开
};


//	TCP连接时回调函数原型
//		ClientSocket：	客户端socket
//		addr：			对方地址信息
//		otherParam:		其他参数
typedef void(*TCPCONNCALLBACK)(SOCKET clientSock, SOCKADDR_IN addr, void *otherParam);

//	TCP/UDP接收到消息时回调函数原型
//		clientSock:		客户端socket
//		addr:			对方地址信息
//		buffer:			接收到的消息
//		len:			接收到的消息长度
typedef void(*TCPRECEIVECALLBACK)(SOCKET clientSock, SOCKADDR_IN addr, const char *buffer, int len, void *otherParam);

//	TCP关闭回调函数原型
//		addr：			对方地址信息
//		otherParam:		其他参数
typedef void(*TCPCLOSECALLBACK)(SOCKADDR_IN addr, void *otherParam);


//初始化WinSock
//返回值：成功返回true，失败返回false
LIBSOCK_API bool SockInit();


//清理WinSock
LIBSOCK_API void SockClean();

//	产生SOCKET
//		type：	要产生的SOCKET类型，TCP或UDP
//	返回值：-1表示失败，其他表示成功
LIBSOCK_API SOCKET MakeSocket(CONNECTTYPE type);

//	绑定套接字到端口
//		sock：	要绑定的套接字
//		port：	要绑定的端口
//	返回值：成功返回true，失败返回false
LIBSOCK_API bool BindSocket(SOCKET sock, const unsigned short port);

//	监听TCP套接字
//		sock：			要监听的套接字
//		callBack:		新连接到来回调函数
//		maxClientNum：	最大连接数
//		otherParam：		其他参数
//	返回值：成功返回true，失败返回false
LIBSOCK_API bool ListenTcpSocket(SOCKET sock, TCPCONNCALLBACK callBack, const int maxClientNum, void *otherParam = 0);

//	发送TCP消息
//		sock：		使用的套接字
//		buffer：		消息缓冲
//		len：		消息长度
//	返回值：成功返回true，失败返回false
LIBSOCK_API bool TCPSend(SOCKET sock, const char*buffer, const int len);

//	接收TCP消息
//		sock：		使用的套接字
//		buffer：		消息缓冲
//		len：		消息长度（同时也是缓冲区长度，使用前需要初始化）
//	返回值：接收错误类型
LIBSOCK_API SOCKERR TCPReceive(SOCKET sock, char*buffer, int *len);

//	发送UDP信息
//		sock：		使用的套接字
//		buffer：		消息缓冲区
//		len：		消息长度
//		addr：		对方地址信息
//		addrLen：	地址信息长度
//	返回值：成功返回true，失败返回false
LIBSOCK_API bool UDPSend(SOCKET sock, const char*buffer, const int len, SOCKADDR *addr, const int addrLen);

//	接收UDP消息
//		sock：		要使用的套接字
//		buffer：		消息缓冲区
//		len：		消息长度（同时也是缓冲区长度，使用前需要初始化）
//		addr：		对方地址信息
//		addrLen：	地址信息长度
//	返回值：成功返回true，失败返回false
LIBSOCK_API bool UDPReceive(SOCKET sock, char*buffer, int *len, SOCKADDR *addr, int addrLen);

//	TCP连接
//		sock：		使用的套接字
//		addr：		对方地址信息
//		addrLen：	地址信息长度
//	返回值：连接成功返回true，失败返回false
LIBSOCK_API bool TCPConnect(SOCKET sock, SOCKADDR *addr, int addrLen);

//	关闭套接字
//		sock：	要关闭的套接字
//	成功关闭返回true，失败返回false
LIBSOCK_API bool CloseSock(SOCKET sock);

//	生成地址信息
//		ipStr：	ip字符串
//		port：	端口
// 返回值：返回地址信息
LIBSOCK_API SOCKADDR_IN MakeAddr(const char* ipStr, const unsigned short port);

//默认的连接回调函数（什么都不做）
LIBSOCK_API void DEFAULTTCPCONNCALLBACK(SOCKET clientSock, SOCKADDR_IN addr, void *otherParam = 0);

//默认的接收回调函数（什么都不做）
LIBSOCK_API void DEFAULTTCPRECEIVECALLBACK(SOCKET clientSock, SOCKADDR_IN addr, const char *buffer, int len, void *otherParam = 0);

//默认的关闭回调函数（什么都不做）
LIBSOCK_API void DEFAULTTCPCLOSECALLBACK(SOCKADDR_IN addr, void *otherParam = 0);

#define UDPRECEIVECALLBACK TCPRECEIVECALLBACK
#define DEFAULTUDPRECEIVECALLBACK DEFAULTTCPRECEIVECALLBACK

//	TCP服务器类
class LIBSOCK_API TCPServer {
public:
	//	使用回调函数初始化
	TCPServer(TCPCONNCALLBACK tcpConnCallBack = DEFAULTTCPCONNCALLBACK,
		TCPRECEIVECALLBACK tcpReceiveCallBack = DEFAULTTCPRECEIVECALLBACK,
		TCPCLOSECALLBACK tcpCloseCallBack = DEFAULTTCPCLOSECALLBACK);

	//	初始化套接字
	//	返回值：初始化成功返回true，失败返回false
	static bool init();

	//	监听套接字
	//		port：			监听的端口
	//		maxClientNum：	最大连接数量
	//		otherParam:		其他参数
	//	返回值：成功返回true，失败返回false
	bool listen(const unsigned short port, int const maxClientNum, void *otherParam = 0);

	//	关闭套接字
	void close();

	//	获取套接字
	//	返回值：实例的套接字
	SOCKET getSock();
private:
	TCPCONNCALLBACK connCb;
	TCPRECEIVECALLBACK receiveCb;
	TCPCLOSECALLBACK closeCb;

	SOCKET sock;
};

//	TCP客户端类
class LIBSOCK_API TCPClient {
public:
	// 见TCP服务器类
	TCPClient(
		TCPCONNCALLBACK tcpConnCallBack = DEFAULTTCPCONNCALLBACK,
		TCPRECEIVECALLBACK tcpReceiveCallBack = DEFAULTTCPRECEIVECALLBACK,
		TCPCLOSECALLBACK tcpCloseCallBack = DEFAULTTCPCLOSECALLBACK);

	// 见TCP服务器类
	static bool init();

	//	连接服务器
	//		ip：			ip字符串
	//		port：		远程端口
	//		selfPort：	本地端口
	//		otherParam:	其他参数
	//	返回值：连接成功返回true，失败返回false
	bool connect(const char*ip, const unsigned short port, const unsigned short selfPort, void *otherParam = 0);

	// 见TCP服务器类
	void close();

	// 见TCP服务器类
	SOCKET getSock();
private:
	TCPCONNCALLBACK connCb;
	TCPRECEIVECALLBACK receiveCb;
	TCPCLOSECALLBACK closeCb;
	SOCKET sock;
};

//	UDP套接字类
class LIBSOCK_API UDPSocket {
public:

	// 见TCP服务器类
	UDPSocket(UDPRECEIVECALLBACK UdpReceiveCallBack = DEFAULTUDPRECEIVECALLBACK);

	// 见TCP服务器类
	static bool init();

	//	接收消息
	//		port：			本地端口
	//		otherParam:		其他参数
	//	返回值：成功返回true，失败返回false
	bool recv(const unsigned short port, void*otherParam = 0);

	// 见TCP服务器类
	void close();

	// 见TCP服务器类
	SOCKET getSock();
private:
	TCPRECEIVECALLBACK receiveCb;
	SOCKET sock;
};


#endif