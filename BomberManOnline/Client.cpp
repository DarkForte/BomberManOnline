#include "StdAfx.h"
#include "Client.h"

using namespace std;

#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}

CClient::CClient(void):
			m_strServerIP(DEFAULT_IP),
			m_strLocalIP(DEFAULT_IP),
			m_nThreads(DEFAULT_THREADS),
			m_nPort(DEFAULT_PORT),
			m_phWorkerThreads(NULL),
			m_hConnectionThread(NULL),
			m_hShutdownEvent(NULL)
{
}

CClient::~CClient(void)
{
	this->Stop();
}

//////////////////////////////////////////////////////////////////////////////////
//	建立连接的线程
DWORD WINAPI CClient::_ConnectionThread(LPVOID lpParam)
{
	THREADPARAMS_CONNECTION* pParams = (THREADPARAMS_CONNECTION*) lpParam;
	CClient* pClient = (CClient*)pParams->pClient;

	OutputDebugPrintf("_AccpetThread启动，系统监听中...\n");

	pClient->EstablishConnections();

	OutputDebugPrintf("_ConnectionThread线程结束.\n");

	RELEASE(pParams);	

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// 发送消息
CMessage  CClient::_SendMessage(CMessage msg)
{
	char szRecv[MAX_BUFFER_LEN];
	memset(szRecv, 0, MAX_BUFFER_LEN);

	int nBytesSent = 0;
	int nBytesRecv = 0;

	// 向服务器发送信息

	nBytesSent = send(m_pParamsWorker->sock, (char*)&msg, sizeof(CMessage), 0);
	if (SOCKET_ERROR == nBytesSent)
	{
		OutputDebugPrintf("错误：发送1次信息失败，错误代码：%ld\n", WSAGetLastError());
	}

	CMessage recv_msg;
	recv(m_pParamsWorker->sock, (char*)&recv_msg, sizeof(CMessage), 0);

	return recv_msg;
}

///////////////////////////////////////////////////////////////////////////////////
// 建立连接
bool  CClient::EstablishConnections()
{
	m_phWorkerThreads = new HANDLE[m_nThreads];
	m_pParamsWorker = new THREADPARAMS_WORKER[m_nThreads];

	// 根据用户设置的线程数量
	for (int i=0; i<m_nThreads; i++)
	{
		// 监听用户的停止事件
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hShutdownEvent, 0))
		{
			OutputDebugPrintf("接收到用户停止命令.\n");
			state = ClientState::DISCONNECT;
			return true;
		}
		
		// 向服务器进行连接
		if( !this->ConnetToServer(&m_pParamsWorker[i].sock,m_strServerIP,m_nPort) )
		{
			OutputDebugPrintf("connect server fail!\n");
			CleanUp();
			state = ClientState::DISCONNECT;
			return false;
		}

		state = ClientState::CONNECT;

		m_pParamsWorker[i].nThreadNo = i+1;

		Sleep(10);

		// 如果连接服务器成功，就开始建立工作者线程，向服务器发送指定数据
		m_pParamsWorker[i].pClient = this;
		//m_phWorkerThreads[i] = CreateThread(0, 0, _WorkerThread, (void *)(&m_pParamsWorker[i]), 0, &nThreadID);
		
		CMessage msg, recv_msg;
		msg.type1 = MSG_NULL;
		recv_msg = _SendMessage(msg);

	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//	向服务器进行Socket连接
bool CClient::ConnetToServer( SOCKET *pSocket, CString strServer, int nPort )
{
	struct sockaddr_in ServerAddress;
	struct hostent *Server;

	// 生成SOCKET
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket) 
	{
		OutputDebugPrintf("错误：初始化Socket失败，错误信息：%d\n", WSAGetLastError());
		return false;
	}

	// 生成地址信息
	size_t len = wcslen(strServer.GetString()) + 1;
	size_t converted = 0;
	char *CStr;
	char cs[20]="";
	CStr = cs;
	wcstombs_s(&converted, CStr, len, strServer.GetString(), _TRUNCATE);
	Server = gethostbyname(CStr);
	if (Server == NULL) 
	{
		closesocket(*pSocket);
		OutputDebugPrintf("错误：无效的服务器地址.\n");
		return false; 
	}
	
	ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr, 
		       (char *)Server->h_addr,
		        Server->h_length);

	ServerAddress.sin_port = htons(m_nPort);

	// 开始连接服务器
	if (SOCKET_ERROR == connect(*pSocket, reinterpret_cast<const struct sockaddr *>(&ServerAddress),sizeof(ServerAddress))) 
	{
		closesocket(*pSocket);
		OutputDebugPrintf("错误：连接至服务器失败！\n");
		return false; 
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// 初始化WinSock 2.2
bool CClient::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		OutputDebugPrintf("init WinSock 2.2 fail!\n");
		return false; // 错误
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// 开始监听
bool CClient::Start()
{
	// 建立系统退出的事件通知
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	state = ClientState::DISCONNECT;

	// 启动连接线程
	DWORD nThreadID;
	THREADPARAMS_CONNECTION* pThreadParams = new THREADPARAMS_CONNECTION;
	pThreadParams->pClient = this;
	m_hConnectionThread = ::CreateThread(0, 0, _ConnectionThread, (void *)pThreadParams, 0, &nThreadID);

	return true;
}

///////////////////////////////////////////////////////////////////////
//	停止监听
void CClient::Stop()
{
	if( m_hShutdownEvent==NULL ) return ;

	SetEvent(m_hShutdownEvent);
	// 等待Connection线程退出
	WaitForSingleObject(m_hConnectionThread, INFINITE);

	// 关闭所有的Socket
	for (int i= 0; i< m_nThreads; i++)
	{
		closesocket(m_pParamsWorker[i].sock);
	}

	// 等待所有的工作者线程退出
	WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

	// 清空资源
	CleanUp();

	state = ClientState::DISCONNECT;

	OutputDebugPrintf("停止.\n");
}

//////////////////////////////////////////////////////////////////////
//	清空资源
void CClient::CleanUp()
{
	if(m_hShutdownEvent==NULL) return;

	RELEASE(m_phWorkerThreads);

	RELEASE_HANDLE(m_hConnectionThread);

	RELEASE(m_pParamsWorker);

	RELEASE_HANDLE(m_hShutdownEvent);
}

////////////////////////////////////////////////////////////////////
// 获得本机的IP地址
CString CClient::GetLocalIP()
{
	char hostname[MAX_PATH];
	gethostname(hostname,MAX_PATH);                // 获得本机主机名

	struct hostent FAR* lpHostEnt = gethostbyname(hostname);

	if(lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      // 取得IP地址列表中的第一个为返回的IP

	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);

	m_strLocalIP = CString( inet_ntoa(inAddr) );   // 转化成标准的IP地址形式

	return m_strLocalIP;
}
