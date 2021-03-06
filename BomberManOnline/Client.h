#pragma once

#include "Message.h"

// 屏蔽deprecation警告
#pragma warning(disable: 4996)

// 缓冲区长度(8*1024字节)
#define MAX_BUFFER_LEN 8196    
#define DEFAULT_PORT          12345                      // 默认端口
#define DEFAULT_IP            _T("127.0.0.1")            // 默认IP地址
#define DEFAULT_THREADS       1                          // 默认并发线程数

class CClient;

// 用于发送数据的线程参数
typedef struct _tagThreadParams_WORKER
{
	CClient* pClient;                               // 类指针，用于调用类中的函数
	SOCKET   sock;                                  // 每个线程使用的Socket
	int      nThreadNo;                             // 线程编号
	char     szBuffer[MAX_BUFFER_LEN];

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER;  

// 产生Socket连接的线程
typedef struct _tagThreadParams_CONNECTION
{
	CClient* pClient;                               // 类指针，用于调用类中的函数

} THREADPARAMS_CONNECTION,*PTHREADPARAM_CONNECTION; 

enum ClientState
{
	DISCONNECT, CONNECT
};

class CClient
{
public:
	CClient(void);
	~CClient(void);

public:

	// 加载Socket库
	bool LoadSocketLib();
	// 卸载Socket库
	void UnloadSocketLib() { WSACleanup(); }

	// 开始
	bool Start();
	//	停止
	void Stop();

	// 获得本机的IP地址
	CString GetLocalIP();

	// 设置连接IP地址
	void SetIP( const CString& strIP ) { m_strServerIP=strIP; }
	// 设置监听端口
	void SetPort( const int& nPort )   { m_nPort=nPort; }
	// 设置并发线程数量
	void SetThreads(const int& n)      { m_nThreads=n; }

	CMessage SendMessage(CMessage msg);

	ClientState state;

	CString GetIp(){ return m_strServerIP; }
	int GetPort(){ return m_nPort; }

private:

	// 建立连接
	bool EstablishConnections();
	// 向服务器进行连接
	bool ConnetToServer( SOCKET *pSocket, CString strServer, int nPort );
	// 用于建立连接的线程
	static DWORD WINAPI _ConnectionThread(LPVOID lpParam);

	// 释放资源
	void CleanUp();

private:

	CString   m_strServerIP;                                // 服务器端的IP地址
	CString   m_strLocalIP;                                 // 本机IP地址
	int       m_nPort;                                      // 监听端口
	int       m_nThreads;                                   // 并发线程数量

	HANDLE    *m_phWorkerThreads;
	HANDLE    m_hConnectionThread;                          // 接受连接的线程句柄
	HANDLE    m_hShutdownEvent;                             // 用来通知线程系统退出的事件，为了能够更好的退出线程

	THREADPARAMS_WORKER      *m_pParamsWorker;              // 线程参数
};
