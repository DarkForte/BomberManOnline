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
			m_strMessage(DEFAULT_MESSAGE),
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
//	�������ӵ��߳�
DWORD WINAPI CClient::_ConnectionThread(LPVOID lpParam)
{
	THREADPARAMS_CONNECTION* pParams = (THREADPARAMS_CONNECTION*) lpParam;
	CClient* pClient = (CClient*)pParams->pClient;

	//TRACE("_AccpetThread������ϵͳ������...\n");

	pClient->EstablishConnections();

	//TRACE(_T("_ConnectionThread�߳̽���.\n"));

	RELEASE(pParams);	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
// ���ڷ�����Ϣ���߳�
DWORD WINAPI CClient::_WorkerThread(LPVOID lpParam)
{
	THREADPARAMS_WORKER *pParams = (THREADPARAMS_WORKER *)lpParam;
	CClient* pClient = (CClient*) pParams->pClient;

	char szTemp[MAX_BUFFER_LEN];
	memset( szTemp,0,sizeof(szTemp) );
	char szRecv[MAX_BUFFER_LEN];
	memset(szRecv,0,MAX_BUFFER_LEN);

	int nBytesSent = 0;
	int nBytesRecv = 0;

	//CopyMemory(szTemp,pParams->szBuffer,sizeof(pParams->szBuffer));

	// �������������Ϣ
	sprintf( szTemp,("Connect"));
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		//TRACE("���󣺷���1����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	
	//TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	OutputDebugPrintf("send message to server success: %s\n", szTemp);

	Sleep( 3000 );

	// �ٷ���һ����Ϣ
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("Message 2"));
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		//TRACE("���󣺷��͵�2����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	
	
	//TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	OutputDebugPrintf("send message to server success: %s\n", szTemp);

	Sleep( 3000 );
	
	// ����3����Ϣ
	memset( szTemp,0,sizeof(szTemp) );
	sprintf( szTemp,("Message 3"));
	nBytesSent = send(pParams->sock, szTemp, strlen(szTemp), 0);
	if (SOCKET_ERROR == nBytesSent) 
	{
		//TRACE("���󣺷��͵�3����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
		return 1; 
	}	

	//TRACE("�������������Ϣ�ɹ�: %s\n", szTemp);
	OutputDebugPrintf("send message to server success: %s\n", szTemp);

	return 0;
}

CMessage  CClient::_SendMessage(CMessage msg)
{
	char szRecv[MAX_BUFFER_LEN];
	memset(szRecv, 0, MAX_BUFFER_LEN);

	int nBytesSent = 0;
	int nBytesRecv = 0;

	//CopyMemory(szTemp,pParams->szBuffer,sizeof(pParams->szBuffer));

	// �������������Ϣ

	nBytesSent = send(m_pParamsWorker->sock, (char*)&msg, sizeof(CMessage), 0);
	if (SOCKET_ERROR == nBytesSent)
	{
		OutputDebugPrintf("���󣺷���1����Ϣʧ�ܣ�������룺%ld\n", WSAGetLastError());
	}

	CMessage recv_msg;
	recv(m_pParamsWorker->sock, (char*)&recv_msg, sizeof(CMessage), 0);

	return recv_msg;

	//OutputDebugPrintf("send message to server success: %s \n", msg.str1);
}

///////////////////////////////////////////////////////////////////////////////////
// ��������
bool  CClient::EstablishConnections()
{
	DWORD nThreadID;

	m_phWorkerThreads = new HANDLE[m_nThreads];
	m_pParamsWorker = new THREADPARAMS_WORKER[m_nThreads];

	// �����û����õ��߳�����������ÿһ���߳����������������������̷߳�������
	for (int i=0; i<m_nThreads; i++)
	{
		// �����û���ֹͣ�¼�
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hShutdownEvent, 0))
		{
			//TRACE(_T("���յ��û�ֹͣ����.\n"));
			return true;
		}
		
		// ���������������
		if( !this->ConnetToServer(&m_pParamsWorker[i].sock,m_strServerIP,m_nPort) )
		{
			OutputDebugPrintf("connect server fail!\n");
			CleanUp();
			return false;
		}

		m_pParamsWorker[i].nThreadNo = i+1;
		sprintf(m_pParamsWorker[i].szBuffer, "%d thread send message %s", i+1, m_strMessage.GetString() );

		Sleep(10);

		// ������ӷ������ɹ����Ϳ�ʼ�����������̣߳������������ָ������
		m_pParamsWorker[i].pClient = this;
		//m_phWorkerThreads[i] = CreateThread(0, 0, _WorkerThread, (void *)(&m_pParamsWorker[i]), 0, &nThreadID);
		
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
//	�����������Socket����
bool CClient::ConnetToServer( SOCKET *pSocket, CString strServer, int nPort )
{
	struct sockaddr_in ServerAddress;
	struct hostent *Server;

	// ����SOCKET
	*pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == *pSocket) 
	{
		//TRACE("���󣺳�ʼ��Socketʧ�ܣ�������Ϣ��%d\n", WSAGetLastError());
		return false;
	}

	// ���ɵ�ַ��Ϣ
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
		//TRACE("������Ч�ķ�������ַ.\n");
		return false; 
	}

	
	ZeroMemory((char *) &ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	CopyMemory((char *)&ServerAddress.sin_addr.s_addr, 
		       (char *)Server->h_addr,
		        Server->h_length);

	ServerAddress.sin_port = htons(m_nPort);

	// ��ʼ���ӷ�����
	if (SOCKET_ERROR == connect(*pSocket, reinterpret_cast<const struct sockaddr *>(&ServerAddress),sizeof(ServerAddress))) 
	{
		closesocket(*pSocket);
		//TRACE("����������������ʧ�ܣ�\n");
		return false; 
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// ��ʼ��WinSock 2.2
bool CClient::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		OutputDebugPrintf("init WinSock 2.2 fail!\n");
		return false; // ����
	}

	return true;
}

///////////////////////////////////////////////////////////////////
// ��ʼ����
bool CClient::Start()
{
	// ����ϵͳ�˳����¼�֪ͨ
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// ���������߳�
	DWORD nThreadID;
	THREADPARAMS_CONNECTION* pThreadParams = new THREADPARAMS_CONNECTION;
	pThreadParams->pClient = this;
	m_hConnectionThread = ::CreateThread(0, 0, _ConnectionThread, (void *)pThreadParams, 0, &nThreadID);

	return true;
}

///////////////////////////////////////////////////////////////////////
//	ֹͣ����
void CClient::Stop()
{
	if( m_hShutdownEvent==NULL ) return ;

	SetEvent(m_hShutdownEvent);
	// �ȴ�Connection�߳��˳�
	WaitForSingleObject(m_hConnectionThread, INFINITE);

	// �ر����е�Socket
	for (int i= 0; i< m_nThreads; i++)
	{
		closesocket(m_pParamsWorker[i].sock);
	}

	// �ȴ����еĹ������߳��˳�
	WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

	// �����Դ
	CleanUp();

	//TRACE("����ֹͣ.\n");
}

//////////////////////////////////////////////////////////////////////
//	�����Դ
void CClient::CleanUp()
{
	if(m_hShutdownEvent==NULL) return;

	RELEASE(m_phWorkerThreads);

	RELEASE_HANDLE(m_hConnectionThread);

	RELEASE(m_pParamsWorker);

	RELEASE_HANDLE(m_hShutdownEvent);
}

////////////////////////////////////////////////////////////////////
// ��ñ�����IP��ַ
CString CClient::GetLocalIP()
{
	char hostname[MAX_PATH];
	gethostname(hostname,MAX_PATH);                // ��ñ���������

	struct hostent FAR* lpHostEnt = gethostbyname(hostname);

	if(lpHostEnt == NULL)
	{
		return DEFAULT_IP;
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      // ȡ��IP��ַ�б��еĵ�һ��Ϊ���ص�IP

	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);

	m_strLocalIP = CString( inet_ntoa(inAddr) );   // ת���ɱ�׼��IP��ַ��ʽ

	return m_strLocalIP;
}
